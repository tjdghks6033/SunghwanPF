#include "Framework.h"
#include "Terrain.h"
#include <random>
#include "Utilities/BinaryFile.h"

Terrain::Terrain(Shader * shader, wstring heightFile)
	: Renderer(shader)
{
	sBaseMap = shader->AsSRV("BaseMap");
	sAlphaMap = shader->AsSRV("AlphaMap");
	sLayerMap = shader->AsSRV("LayerMap");
	sLayerMap2 = shader->AsSRV("LayerMap2");
	sLayerMap3 = shader->AsSRV("LayerMap3");

	heightMap = new Texture(heightFile);

	brushBuffer = new ConstantBuffer(&brushDesc, sizeof(BrushDesc));
	sBrushBuffer = shader->AsConstantBuffer("CB_Brush");

	lineBuffer = new ConstantBuffer(&lineDesc, sizeof(LineDesc));
	sLineBuffer = shader->AsConstantBuffer("CB_TerrainLine");

	CreateVertexData();
	CreateIndexData();
	CreateNormalData();

	//vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(TerrainVertex), 0, false, true);
	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(TerrainVertex), 0, true);
	indexBuffer = new IndexBuffer(indices, indexCount);
}

Terrain::~Terrain()
{
	SafeDelete(brushBuffer);
	SafeDelete(lineBuffer);
	SafeDelete(heightMap);

	SafeDelete(baseMap);
	SafeDelete(layerMap);
	SafeDelete(layerMap2);
	SafeDelete(layerMap3);
	SafeDelete(alphaMap);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);
}

void Terrain::Update()
{
	Super::Update();

	
	ImGui::Checkbox("Map Editor", &is_mapeditor);

	if (is_mapeditor)
	{
		ImGui::InputInt("Type", (int *)&brushDesc.Type);
		brushDesc.Type %= 3;

		ImGui::InputInt("Range", (int *)&brushDesc.Range);
		brushDesc.Range %= 20;

		ImGui::Checkbox("Raise Height", &is_raise);

		ImGui::Checkbox("Flattening", &is_flattening);

		ImGui::Checkbox("Smoothing", &is_smoothing);

		ImGui::Checkbox("Slope", &is_slope);

		ImGui::Checkbox("Noise", &is_noise);

		if (is_noise)
		{
			ImGui::SliderFloat("Noise Height", &noise_var, 1.0f, 10.0f);
			ImGui::SliderInt("Noise Frequency", &noise_freq, 1, 10);
		}

		ImGui::Checkbox("Paint", &is_paint);

		ImGui::SliderInt("Layer Num", &layernum, 1, 3);

		std::wstring file;
		if (ImGui::Button("Save"))
		{
			Path::SaveFileDialog(file, Path::ImageFilter, L"../../Textures/Save/TerrainMap/", [=](std::wstring path) {

				auto writer = new BinaryWriter();
				writer->Open(path, 1);
				for (UINT z = 0; z < height - 1; z++)
				{
					for (UINT x = 0; x < width - 1; x++)
					{
						float temp = GetHeight(Vector3((float)x, 0, (float)z));
						writer->Float(temp);

						float r = vertices[width * z + x].Color.r;
						float g = vertices[width * z + x].Color.g;
						float b = vertices[width * z + x].Color.b;

						writer->Float(r);
						writer->Float(g);
						writer->Float(b);
					}
				}
				writer->Close();
			});
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			Path::OpenFileDialog(file, Path::ImageFilter, L"../../Textures/Save/TerrainMap/", [=](std::wstring path) {
				auto reader = new BinaryReader();

				reader->Open(path);
				for (UINT z = 0; z < height - 1; z++)
				{
					for (UINT x = 0; x < width - 1; x++)
					{
						float temp = reader->Float();
						float r = reader->Float();
						float g = reader->Float();
						float b = reader->Float();

						vertices[width * z + x].Position.y = temp;
						vertices[width * z + x].Color = Color(r, g, b, 1);

					}

				}
				CreateNormalData();
				D3D11_MAPPED_SUBRESOURCE subResource;
				D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
				{
					memcpy(subResource.pData, vertices, sizeof(TerrainVertex) * vertexCount);
				}
				D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);

				reader->Close();
			});
		}


		if (brushDesc.Type > 0)
		{
			

			if (!is_clicked)
				brushDesc.Location = GetPickedPosition();

			if (Mouse::Get()->Down(0))
				prevlocation = brushDesc.Location;

			if (Mouse::Get()->Press(0))
			{
				ImVec2 aa = ImGui::GetMousePos();

				if (aa.x >= 0.0f && aa.x < 362.0f && aa.y > 250.0f && aa.y <= 620)
					is_hovered = true;
				else
					is_hovered = false;
				
				if (!is_hovered)
				{
					if (is_flattening)
						FlatHeight(brushDesc.Location, brushDesc.Type, brushDesc.Range);
					else if (is_smoothing)
						Smoothing(brushDesc.Location, brushDesc.Type, brushDesc.Range);
					else if (is_noise)
						Noise(brushDesc.Location, brushDesc.Type, brushDesc.Range);
					else if (is_slope)
						Slope(brushDesc.Location, brushDesc.Type, brushDesc.Range);
					else if (is_raise)
					{
						RaiseHeight(brushDesc.Location, brushDesc.Type, brushDesc.Range);
						//is_clicked = true;
					}
					else if (is_paint)
					{
						Paint(brushDesc.Location, brushDesc.Type, brushDesc.Range);
					}
				}
			}

			if (Keyboard::Get()->Press(VK_SHIFT))
			{
				DownHeight(brushDesc.Location, brushDesc.Type, brushDesc.Range);
				//is_clicked = true;
			}
			/*if (Mouse::Get()->Up(0))
				is_clicked = false;
			*/
		}
	}

	ImGui::Separator();

	/*ImGui::ColorEdit3("Color", lineDesc.Color);

	ImGui::InputInt("Visible", (int *)&lineDesc.Visible);
	lineDesc.Visible %= 2;

	ImGui::InputFloat("Thickness", &lineDesc.Thickness, 0.001f);
	lineDesc.Thickness = Math::Clamp(lineDesc.Thickness, 0.01f, 0.9f);

	ImGui::InputFloat("Size", &lineDesc.Size, 1.0f);*/
}

void Terrain::Render()
{
	Super::Render();

	if (baseMap != NULL)
		sBaseMap->SetResource(baseMap->SRV());

	brushBuffer->Apply();
	sBrushBuffer->SetConstantBuffer(brushBuffer->Buffer());

	lineBuffer->Apply();
	sLineBuffer->SetConstantBuffer(lineBuffer->Buffer());


	if (layerMap != NULL && alphaMap != NULL)
	{
		sAlphaMap->SetResource(alphaMap->SRV());
		sLayerMap->SetResource(layerMap->SRV());
		sLayerMap2->SetResource(layerMap2->SRV());
		sLayerMap3->SetResource(layerMap3->SRV());
	}

	shader->DrawIndexed(0, Pass(), indexCount);
}

void Terrain::BaseMap(wstring file)
{
	SafeDelete(baseMap);

	baseMap = new Texture(file);
}

void Terrain::LayerMap(wstring file, wstring alpha)
{
	SafeDelete(alphaMap);
	SafeDelete(layerMap);

	alphaMap = new Texture(alpha);
	layerMap = new Texture(file);
}

void Terrain::LayerMap2(wstring file, wstring alpha)
{
	SafeDelete(alphaMap);
	SafeDelete(layerMap2);

	alphaMap = new Texture(alpha);
	layerMap2 = new Texture(file);
}

void Terrain::LayerMap3(wstring file, wstring alpha)
{
	SafeDelete(alphaMap);
	SafeDelete(layerMap3);

	alphaMap = new Texture(alpha);
	layerMap3 = new Texture(file);
}

float Terrain::GetHeight(Vector3 & position)
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

float Terrain::GetHeightPick(Vector3 & position)
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

Vector3 Terrain::GetPickedPosition()
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

void Terrain::RaiseHeight(Vector3 & position, UINT type, UINT range)
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
		memcpy(subResource.pData, vertices, sizeof(TerrainVertex) * vertexCount);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
}

void Terrain::DownHeight(Vector3 & position, UINT type, UINT range)
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

		for (float z = (float)rect.bottom; z <= (float)rect.top; z++)
		{
			for (float x = (float)rect.left; x <= (float)rect.right; x++)
			{
				UINT index = width * (UINT)z + (UINT)x;

				vertices[index].Position.y -= 5.0f * Time::Delta();
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

		for (float z = (float)rect.bottom; z <= (float)rect.top; z++)
		{
			for (float x = (float)rect.left; x <= (float)rect.right; x++)
			{
				if ((position.x - x) *(position.x - x) + (position.z - z) * (position.z - z) <= range * range)
				{
					UINT index = width * (UINT)z + (UINT)x;
					vertices[index].Position.y -= 5.0f * Time::Delta();
				}
			}
		}
		CreateNormalData();
	}

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(TerrainVertex) * vertexCount);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
}

void Terrain::FlatHeight(Vector3 & position, UINT type, UINT range)
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

		for (float z = (float)rect.bottom; z <= (float)rect.top; z++)
		{
			for (float x = (float)rect.left; x <= (float)rect.right; x++)
			{
				UINT index = width * (UINT)z + (UINT)x;

				vertices[index].Position.y = 0.3f * Time::Delta();
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

		for (float z = (float)rect.bottom; z < (float)rect.top; z++)
		{
			for (float x = (float)rect.left; x < (float)rect.right; x++)
			{
				if ((position.x - x) *(position.x - x) + (position.z - z) * (position.z - z) <= range * range)
				{
					UINT index = width * (UINT)z + (UINT)x;
					vertices[index].Position.y = 0.3f * Time::Delta();
				}
			}
		}
		CreateNormalData();
	}

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(TerrainVertex) * vertexCount);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
}

void Terrain::Smoothing(Vector3 & position, UINT type, UINT range)
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

		for (float z = (float)rect.bottom; z <= (float)rect.top; z++)
		{
			for (float x = (float)rect.left; x <= (float)rect.right; x++)
			{
				UINT index = width * (UINT)z + (UINT)x; //중점
				UINT indexlt = width * ((UINT)z + 1) + (UINT)x - 1; //lt
				UINT indextc = width * ((UINT)z + 1) + (UINT)x; //tc
				UINT indexrt = width * ((UINT)z + 1) + (UINT)x + 1;	//rt
				UINT indexlc = width * (UINT)z + (UINT)x - 1;	//lc
				UINT indexrc = width * (UINT)z + (UINT)x + 1;	//rc
				UINT indexlb = width * ((UINT)z - 1) + (UINT)x - 1;	//lb
				UINT indexcb = width * ((UINT)z - 1) + (UINT)x;	//cb
				UINT indexrb = width * ((UINT)z - 1) + (UINT)x + 1;	//rb
				

				float cc = vertices[index].Position.y;
				float lt = vertices[indexlt].Position.y;
				float tc = vertices[indextc].Position.y;
				float rt = vertices[indexrt].Position.y;
				float lc = vertices[indexlc].Position.y;
				float rc = vertices[indexrc].Position.y;
				float lb = vertices[indexlb].Position.y;
				float cb = vertices[indexcb].Position.y;
				float rb = vertices[indexrb].Position.y;

				float average = (lt + tc + rt + lc + rc + lb + cb + rb) / 8.0f;

				vertices[index].Position.y = ((vertices[index].Position.y + average) / 2.0f);
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

		for (float z = (float)rect.bottom; z < (float)rect.top; z++)
		{
			for (float x = (float)rect.left; x < (float)rect.right; x++)
			{
				if ((position.x - x) *(position.x - x) + (position.z - z) * (position.z - z) <= range * range)
				{
					UINT index = width * (UINT)z + (UINT)x; //중점
					UINT indexlt = width * ((UINT)z + 1) + (UINT)x - 1; //lt
					UINT indextc = width * ((UINT)z + 1) + (UINT)x; //tc
					UINT indexrt = width * ((UINT)z + 1) + (UINT)x + 1;	//rt
					UINT indexlc = width * (UINT)z + (UINT)x - 1;	//lc
					UINT indexrc = width * (UINT)z + (UINT)x + 1;	//rc
					UINT indexlb = width * ((UINT)z - 1) + (UINT)x - 1;	//lb
					UINT indexcb = width * ((UINT)z - 1) + (UINT)x;	//cb
					UINT indexrb = width * ((UINT)z - 1) + (UINT)x + 1;	//rb

					

					float cc = vertices[index].Position.y  ;
					float lt = vertices[indexlt].Position.y;
					float tc = vertices[indextc].Position.y;
					float rt = vertices[indexrt].Position.y;
					float lc = vertices[indexlc].Position.y;
					float rc = vertices[indexrc].Position.y;
					float lb = vertices[indexlb].Position.y;
					float cb = vertices[indexcb].Position.y;
					float rb = vertices[indexrb].Position.y;

					float average = (lt + tc + rt + lc + rc + lb + cb + rb) / 8.0f;

					vertices[index].Position.y = ((vertices[index].Position.y + average) / 2.0f);
				}
			}
		}
		CreateNormalData();
	}

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(TerrainVertex) * vertexCount);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
}

void Terrain::Slope(Vector3 & position, UINT type, UINT range)
{
	if (prevlocation == Vector3(0.0f, 0.0f, 0.0f))
		prevlocation = position;

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

		position.y = 20.0f;
		float dy = (position.y - prevlocation.y) / 100.0f;

		for (float z = prevlocation.z; z <= (float)rect.top; z++)
		{
			for (float x = prevlocation.x; x <= (float)rect.right; x++)
			{
				UINT index = width * (UINT)z + (UINT)x;

				vertices[index].Position.y = dy;
			}
			dy++;
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

		position.y = 20.0f;
		float dy = (position.y - prevlocation.y) / 100.0f;

		for (float z = prevlocation.z; z <= rect.top; z++)
		{
			for (float x = prevlocation.x; x <= rect.right; x++)
			{
				if ((position.x - x) *(position.x - x) + (position.z - z) * (position.z - z) <= range * range)
				{
					UINT index = width * (UINT)z + (UINT)x;

					vertices[index].Position.y = dy;
				}
			}
			dy++;
		}
		CreateNormalData();
	}

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(TerrainVertex) * vertexCount);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
}

void Terrain::Noise(Vector3 & position, UINT type, UINT range)
{
	mt19937 engine((unsigned int)time(NULL));                    // MT19937 난수 엔진
	uniform_real_distribution<float> distribution(0.0f, noise_var);       // 생성 범위
	auto generator = bind(distribution, engine);

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

		for (float z = (float)rect.bottom; z <= (float)rect.top; z++)
		{
			for (float x = (float)rect.left; x <= (float)rect.right; x++)
			{
				UINT index = width * (UINT)z + (UINT)x;

				if (noise_freq > 1)
					if (index % noise_freq == 0)
						continue;

				vertices[index].Position.y = generator();
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

		for (float z = (float)rect.bottom; z < (float)rect.top; z++)
		{
			for (float x = (float)rect.left; x < (float)rect.right; x++)
			{
				if ((position.x - x) *(position.x - x) + (position.z - z) * (position.z - z) <= range * range)
				{
					UINT index = width * (UINT)z + (UINT)x;

					vertices[index].Position.y = generator();
				}
			}
		}
		CreateNormalData();
	}

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(TerrainVertex) * vertexCount);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
}

void Terrain::Paint(Vector3 & position, UINT type, UINT range)
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

		for (float z = (float)rect.bottom; z <= (float)rect.top; z++)
		{
			for (float x = (float)rect.left; x <= (float)rect.right; x++)
			{
				UINT index = width * (UINT)z + (UINT)x;

				if (layernum == 1)
				{
					vertices[index].Color = Color(1, 0, 0, 1);
				}
				else if (layernum == 2)
				{
					vertices[index].Color = Color(0, 1, 0, 1);
				}
				else if (layernum == 3)
				{
					vertices[index].Color = Color(0, 0, 1, 1);
				}

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

		for (float z = (float)rect.bottom; z <= (float)rect.top; z++)
		{
			for (float x = (float)rect.left; x <= (float)rect.right; x++)
			{
				if ((position.x - x) *(position.x - x) + (position.z - z) * (position.z - z) <= range * range)
				{
					UINT index = width * (UINT)z + (UINT)x;

					if (layernum == 1)
					{
						vertices[index].Color = Color(1, 0, 0, 1);
					}
					else if (layernum == 2)
					{
						vertices[index].Color = Color(0, 1, 0, 1);
					}
					else if (layernum == 3)
					{
						vertices[index].Color = Color(0, 0, 1, 1);
					}
				}
			}
		}
		CreateNormalData();
	}

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(TerrainVertex) * vertexCount);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);
}

void Terrain::CreateVertexData()
{
	vector<Color> heights;
	heightMap->ReadPixel(DXGI_FORMAT_R8G8B8A8_UNORM, &heights);

	width = heightMap->GetWidth();
	height = heightMap->GetHeight();

	vertexCount = width * height;
	vertices = new TerrainVertex[vertexCount];
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index = width * z + x;
			UINT pixel = width * (height - 1 - z) + x;

			vertices[index].Position.x = (float)x;
			vertices[index].Position.y = (heights[pixel].r * 255.0f) / 10.0f;
			vertices[index].Position.z = (float)z;

			vertices[index].Uv.x = 5 * (float)x / (float)width;
			vertices[index].Uv.y = 5 * (float)(height - 1 - z) / (float)height;
		}
	}
}

void Terrain::CreateIndexData()
{
	indexCount = (width - 1) * (height - 1) * 6;

	indices = new UINT[indexCount];

	UINT index = 0;
	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices[index + 0] = width * z + x;
			indices[index + 1] = width * (z + 1) + x;
			indices[index + 2] = width * z + x + 1;
			indices[index + 3] = width * z + x + 1;
			indices[index + 4] = width * (z + 1) + x;
			indices[index + 5] = width * (z + 1) + x + 1;

			index += 6;
		}
	}
}

void Terrain::CreateNormalData()
{
	for (UINT i = 0; i < indexCount / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		TerrainVertex v0 = vertices[index0];
		TerrainVertex v1 = vertices[index1];
		TerrainVertex v2 = vertices[index2];

		Vector3 d1 = v1.Position - v0.Position;
		Vector3 d2 = v2.Position - v0.Position;

		Vector3 normal;
		D3DXVec3Cross(&normal, &d1, &d2);

		vertices[index0].Normal = normal;
		vertices[index1].Normal = normal;
		vertices[index2].Normal = normal;
	}

	for (UINT i = 0; i < vertexCount; i++)
		D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
}