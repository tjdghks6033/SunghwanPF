#include "Framework.h"
#include "TerrainLod.h"
#include "Viewer/Fixity.h"

TerrainLod::TerrainLod(InitializeDesc & initDesc)
	: Renderer(initDesc.shader), initDesc(initDesc)
{
	Topology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	sBaseMap = shader->AsSRV("BaseMap");
	sLayerMap = shader->AsSRV("LayerMap");
	sAlphaMap = shader->AsSRV("AlphaMap");
	sHeightMap = shader->AsSRV("HeightMap");
	sNormalMap = shader->AsSRV("NormalMap");

	buffer = new ConstantBuffer(&bufferDesc, sizeof(BufferDesc));
	sBuffer = shader->AsConstantBuffer("CB_TerrainLod");

	heightMap = new Texture(initDesc.heightMap);
	sHeightMap->SetResource(heightMap->SRV());
	heightMap->ReadPixel(DXGI_FORMAT_R8G8B8A8_UINT, &heightMapPixel);

	width = this->heightMap->GetWidth();
	height = this->heightMap->GetHeight();

	vertexPerPatchX = (width / initDesc.CellsPerPatch) + 1;
	vertexPerPatchZ = (height / initDesc.CellsPerPatch) + 1;

	vertexCount = vertexPerPatchX * vertexPerPatchZ;
	faceCount = (vertexPerPatchX - 1) * (vertexPerPatchZ - 1);
	indexCount = faceCount * 4;

	CalcBoundY();
	CreateVertexData();
	CreateIndexData();

	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(VertexTerrain));
	indexBuffer = new IndexBuffer(indices, indexCount);

	bufferDesc.TexelCellSpaceU = 1.0f / ((float)heightMap->GetWidth() - 1.0f);
	bufferDesc.TexelCellSpaceV = 1.0f / ((float)heightMap->GetHeight() - 1.0f);
	bufferDesc.HeightRatio = initDesc.HeightRatio;


	camera = new Fixity();
	perspective = new Perspective(D3D::Width(), D3D::Height(), 0.1f, 1000.0f, Math::PI * 0.35f);

	frustum = new Frustum(NULL, perspective);
	//frustum = new Frustum(camera, perspective);
}

TerrainLod::~TerrainLod()
{
	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(buffer);
	SafeDelete(frustum);
	SafeDelete(camera);
	SafeDelete(perspective);

	SafeDelete(heightMap);

	SafeDelete(baseMap);
	SafeDelete(layerMap);
	SafeDelete(alphaMap);
}

void TerrainLod::Update()
{
	Super::Update();
	ImGui::SliderFloat("MinDistance", &bufferDesc.MinDistance, 1, 100);
	ImGui::SliderFloat("MaxDistance", &bufferDesc.MaxDistance, 1, 1000);

	camera->Update();
	frustum->Update();

	frustum->Planes(bufferDesc.WorldFrustumPlanes);
}

void TerrainLod::Render()
{
	Super::Render();

	if (baseMap != NULL)
		sBaseMap->SetResource(baseMap->SRV());

	if (layerMap != NULL)
		sLayerMap->SetResource(layerMap->SRV());

	if (alphaMap != NULL)
		sAlphaMap->SetResource(alphaMap->SRV());

	if (normalMap != NULL)
		sNormalMap->SetResource(normalMap->SRV());

	buffer->Apply();
	sBuffer->SetConstantBuffer(buffer->Buffer());
	shader->DrawIndexed(0, Pass(), indexCount);
}

void TerrainLod::BaseMap(wstring file)
{	
	SafeDelete(baseMap);

	baseMap = new Texture(file);
}

void TerrainLod::LayerMap(wstring layer, wstring alpha)
{
	SafeDelete(alphaMap);
	SafeDelete(layerMap);

	alphaMap = new Texture(alpha);
	layerMap = new Texture(layer);
}

void TerrainLod::NormalMap(wstring file)
{
	SafeDelete(normalMap);

	normalMap = new Texture(file);
}

float TerrainLod::GetHeight(Vector3 & position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width) return FLT_MIN;
	if (z < 0 || z > height) return FLT_MIN;


	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + x + 1;
	index[3] = width * (z + 1) + x + 1;

	Vector3 v[4];
	for (int i = 0; i < 4; i++)
		v[i] = vertices[index[i]].Position;


	float ddx = (position.x - v[0].x) / 1.0f;
	float ddz = (position.z - v[0].z) / 1.0f;

	Vector3 result;

	if (ddx + ddz <= 1)
		result = v[0] + (v[2] - v[0]) * ddx + (v[1] - v[0]) * ddz;
	else
	{
		ddx = 1 - ddx;
		ddz = 1 - ddz;

		result = v[3] + (v[1] - v[3]) * ddx + (v[2] - v[3]) * ddz;
	}

	return result.y;
}

float TerrainLod::GetHeightPick(Vector3 & position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width) return FLT_MIN;
	if (z < 0 || z > height) return FLT_MIN;


	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + x + 1;
	index[3] = width * (z + 1) + x + 1;

	Vector3 p[4];
	for (int i = 0; i < 4; i++)
		p[i] = vertices[index[i]].Position;


	Vector3 start(position.x, 1000, position.z);
	Vector3 direction(0, -1, 0);

	float u, v, distance;
	Vector3 result(-1, FLT_MIN, -1);
	if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
		result = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

	if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
		result = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;

	return result.y;
}

Vector3 TerrainLod::GetPickedPosition()
{
	Matrix V = Context::Get()->View();
	Matrix P = Context::Get()->Projection();
	Viewport* Vp = Context::Get()->GetViewport();

	Vector3 mouse = Mouse::Get()->GetPosition();

	Vector3 n, f;
	mouse.z = 0.0f;
	Vp->Unproject(&n, mouse, transform->World(), V, P);

	mouse.z = 1.0f;
	Vp->Unproject(&f, mouse, transform->World(), V, P);

	Vector3 start = n;
	Vector3 direction = f - n;

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index[4];
			index[0] = width * z + x;
			index[1] = width * (z + 1) + x;
			index[2] = width * z + x + 1;
			index[3] = width * (z + 1) + x + 1;

			Vector3 p[4];
			for (UINT i = 0; i < 4; i++)
				p[i] = vertices[index[i]].Position;

			float u, v, distance;
			if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
				return p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

			if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
				return p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
		}
	}

	return Vector3(-1, FLT_MIN, -1);
}

void TerrainLod::RaiseHeight(Vector3 & position, UINT type, UINT range)
{
	if (type == 1)
	{
		D3D11_RECT rect;
		rect.left = (LONG)position.x - range;
		rect.top = (LONG)position.z + range;
		rect.right = (LONG)position.x + range;
		rect.bottom = (LONG)position.z - range;


		if (rect.left < 0) rect.left = 0;
		if (rect.top >= (LONG)height) rect.top = (LONG)height;
		if (rect.right >= (LONG)width) rect.right = (LONG)width;
		if (rect.bottom < 0) rect.bottom = 0;

		for (LONG z = rect.bottom; z <= rect.top; z++)
		{
			for (LONG x = rect.left; x <= rect.right; x++)
			{
				UINT index = width * (UINT)z + (UINT)x;

				vertices[index].Position.y += 5.0f * Time::Delta();
			}
		}
		CreateNormalData();
	}

	else if (type == 2)
	{
		D3D11_RECT rect;
		rect.left = (LONG)position.x - range;
		rect.top = (LONG)position.z + range;
		rect.right = (LONG)position.x + range;
		rect.bottom = (LONG)position.z - range;


		if (rect.left < 0) rect.left = 0;
		if (rect.top >= (LONG)height) rect.top = (LONG)height;
		if (rect.right >= (LONG)width) rect.right = (LONG)width;
		if (rect.bottom < 0) rect.bottom = 0;

		for (LONG z = rect.bottom; z <= rect.top; z++)
		{
			for (LONG x = rect.left; x <= rect.right; x++)
			{
				if ((position.x - x) *(position.x - x) + (position.z - z) * (position.z - z) <= range * range)
				{
					UINT index = width * (UINT)z + (UINT)x;
					vertices[index].Position.y += 5.0f * Time::Delta();
				}
			}
		}
		CreateNormalData();		
	}

	//D3D::GetDC()->UpdateSubresource
	//(
	//	vertexBuffer->Buffer(), 0, NULL, vertices, sizeof(TerrainVertex) * vertexCount, 0
	//);

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(VertexTerrain) * vertexCount);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
}

bool TerrainLod::InBounds(UINT x, UINT z)
{
	return x >= 0 && x < width && z >= 0 && z < height;
}

void TerrainLod::CalcPatchBounds(UINT x, UINT z)
{
	UINT x0 = x * initDesc.CellsPerPatch;
	UINT x1 = (x + 1) * initDesc.CellsPerPatch;
	
	UINT z0 = z * initDesc.CellsPerPatch;
	UINT z1 = (z + 1) * initDesc.CellsPerPatch;

	float minY = FLT_MAX;
	float maxY = FLT_MIN;

	for (UINT z = z0; z <= z1; z++)
	{
		for (UINT x = x0; x <= x1; x++)
		{
			float y = 0.0f;
			UINT pixel = width * (height - 1 - z) + x;

			if (InBounds(x, z))
				y = heightMapPixel[pixel].b * 255 / initDesc.HeightRatio;

			minY = min(minY, y);
			maxY = max(maxY, y);
		}
	}

	UINT patchID = (vertexPerPatchX - 1) * z + x;
	bounds[patchID] = Vector2(minY, minY);

}

void TerrainLod::CalcBoundY()
{
	bounds.assign(faceCount, Vector2());

	for (UINT z = 0; z < vertexPerPatchZ - 1; z++)
	{
		for (UINT x = 0; x < vertexPerPatchX - 1; x++)
		{
			CalcPatchBounds(x, z);
		}
	}
}

void TerrainLod::CreateVertexData()
{
	vertices = new VertexTerrain[vertexCount];

	float halfWidth = 0.5f * (float)width;
	float halfHeight = 0.5f * (float)height;

	float patchWidth = (float)width / (float)(vertexPerPatchX - 1);
	float patchHeight = (float)height / (float)(vertexPerPatchZ - 1);

	float du = 1.0f / (float)(vertexPerPatchX - 1);
	float dv = 1.0f / (float)(vertexPerPatchZ - 1);

	for (UINT z = 0; z < vertexPerPatchZ; z++)
	{
		float z1 = halfHeight - (float)z * patchHeight;
		for (UINT x = 0; x < vertexPerPatchX; x++)
		{
			float x1 = -halfWidth + (float)x * patchWidth;
			UINT vertId = z * vertexPerPatchX + x;

			vertices[vertId].Position = Vector3(x1, 0, z1);
			vertices[vertId].Uv = Vector2(x * du, z * dv);
		}
	}

	for (UINT z = 0; z < vertexPerPatchZ - 1; z++)
	{
		for (UINT x = 0; x < vertexPerPatchX - 1; x++)
		{
			UINT patchID = (vertexPerPatchX - 1) * z + x;
			UINT vertID = vertexPerPatchX * z + x;

			vertices[vertID].BoundsY = bounds[patchID];
		}
	}
}

void TerrainLod::CreateIndexData()
{
	UINT index = 0;
	this->indices = new UINT[indexCount];
	vector<UINT> indices;
	for (UINT z = 0; z < vertexPerPatchZ - 1; z++)
	{
		for (UINT x = 0; x < vertexPerPatchX - 1; x++)
		{
			this->indices[index++] = z * vertexPerPatchX + x;
			this->indices[index++] = z * vertexPerPatchX + x + 1;
			this->indices[index++] = (z + 1) * vertexPerPatchX + x;
			this->indices[index++] = (z + 1) * vertexPerPatchX + x + 1;
		}
	}
}


void TerrainLod::CreateNormalData()
{
	for (UINT i = 0; i < indexCount / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexTerrain v0 = vertices[index0];
		VertexTerrain v1 = vertices[index1];
		VertexTerrain v2 = vertices[index2];

		Vector3 d1 = v1.Position - v0.Position;
		Vector3 d2 = v2.Position - v0.Position;

		Vector3 normal;
		D3DXVec3Cross(&normal, &d1, &d2);

		vertices[index0].BoundsY.x = normal.x;
		vertices[index0].BoundsY.y = normal.y;


		vertices[index1].BoundsY.x = normal.x;
		vertices[index1].BoundsY.y = normal.y;


		vertices[index2].BoundsY.x = normal.x;
		vertices[index2].BoundsY.y = normal.y;
	}

	

	for (UINT i = 0; i < vertexCount; i++)
		D3DXVec2Normalize(&vertices[i].BoundsY, &vertices[i].BoundsY);
}