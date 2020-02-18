#include "Framework.h"
#include "Trail.h"

Trail::Trail(InitializeDesc & initDesc, UINT count)
	: Renderer(L"48_Water.fxo"), desc(initDesc), count(count)
{	
	float w = 0.5f;
	float h = 0.5f;
	


	// Create Vertex Buffer
	{
		vertices = new VertexTexture[6 * count];

		float dx = 1.0f / (float)count; // ∫Ò¿≤

		for (UINT i = 0; i < count; i++)
		{
			vertices[i * 6 + 0].Position = Vector3(desc.trail._41 - 0.5f + (dx * i), desc.trail._42 - 0.5f, desc.trail._43 + 0.0f);
			vertices[i * 6 + 1].Position = Vector3(desc.trail._41 - 0.5f + (dx * i), desc.trail._42 + 0.5f, desc.trail._43 + 0.0f);
			vertices[i * 6 + 2].Position = Vector3(desc.trail._41 + 0.5f + dx * (i + 1), desc.trail._42 - 0.5f, desc.trail._43 + 0.0f);
			vertices[i * 6 + 3].Position = Vector3(desc.trail._41 + 0.5f + dx * (i + 1), desc.trail._42 - 0.5f, desc.trail._43 + 0.0f);
			vertices[i * 6 + 4].Position = Vector3(desc.trail._41 - 0.5f + (dx * i), desc.trail._42 + 0.5f, desc.trail._43 + 0.0f);
			vertices[i * 6 + 5].Position = Vector3(desc.trail._41 + 0.5f + dx * (i + 1), desc.trail._42 + 0.5f, desc.trail._43 + 0.0f);

			/*vertices[i * 6 + 0].Uv = Vector2((dx * i), 1);
			vertices[i * 6 + 1].Uv = Vector2((dx * i), 0);
			vertices[i * 6 + 2].Uv = Vector2(dx * (i + 1), 1);
			vertices[i * 6 + 3].Uv = Vector2(dx * (i + 1), 1);
			vertices[i * 6 + 4].Uv = Vector2((dx * i), 0);
			vertices[i * 6 + 5].Uv = Vector2(dx * (i + 1), 0);*/

			vertices[i * 6 + 0].Uv = Vector2(0, 1);
			vertices[i * 6 + 1].Uv = Vector2(0, 0);
			vertices[i * 6 + 2].Uv = Vector2(1, 1);
			vertices[i * 6 + 3].Uv = Vector2(1, 1);
			vertices[i * 6 + 4].Uv = Vector2(0, 0);
			vertices[i * 6 + 5].Uv = Vector2(1, 0);
		}
		vertexBuffer = new VertexBuffer(vertices, count * 6, sizeof(VertexTexture), 0, true);
	}	
	
	texture = new Texture(L"red_lightning.png");

	buffer = new ConstantBuffer(&desc, sizeof(InitializeDesc));
	sBuffer = shader->AsConstantBuffer("CB_Trail");
	sTrailMap = shader->AsSRV("TrailMap");

	
	GenerateTrail();
}

Trail::~Trail()
{
	SafeDeleteArray(vertices);
	SafeDelete(buffer);
}

void Trail::Update(Matrix mat)
{
	Super::Update();
	UpdateTransform(mat);
}

void Trail::Render()
{
	buffer->Apply();
	sBuffer->SetConstantBuffer(buffer->Buffer());

	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	vertexBuffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	Super::Render();
	sTrailMap->SetResource(texture->SRV());
	shader->Draw(0, pass, 6 * count);
}

void Trail::GenerateTrail()
{
	
}

void Trail::UpdateTrailBuffer()
{
	/*D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, &desc.trail, sizeof(VertexTexture) * count);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);*/
}

void Trail::UpdateTransform(Matrix mat)
{
	/*Vector3 scale = Vector3(scale);
	Vector3 translation = Vector3(translation);
	Vector3 rotation = Vector3(rotation);

	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, Math::ToRadian(rotation.y), Math::ToRadian(rotation.x), Math::ToRadian(rotation.z));
	D3DXMatrixTranslation(&T, translation.x, translation.y, translation.z);
	desc.trail = S * R * T;*/
	
	desc.trail = mat;

	Vector3 transdecom;
	Vector3 rotatdecom;
	Math::MatrixDecompose(mat, scale, rotatdecom, transdecom);

	ImGui::SliderFloat3("scale", scale, -10.0f, 10.0f);
	ImGui::SliderFloat3("translation", translation, -10.0f, 10.0f);
	ImGui::SliderFloat3("rotation", rotation, -3.14f, 3.14f);

	transform->Position(Vector3(transdecom.x + translation.x, transdecom.y + translation.y, transdecom.z + translation.z));
	transform->Scale(Vector3(1, 1, 1));
	transform->Rotation(Vector3(rotatdecom.x + rotation.x, rotatdecom.y + rotation.y, rotatdecom.z + rotation.z));

	
	float dx = 1.0f / (float)count;

	for (UINT i = 0; i < count; i++)
	{
		
	}

	/*D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer->Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(VertexTexture) * 6 * count);
	}
	D3D::GetDC()->Unmap(vertexBuffer->Buffer(), 0);*/

	UpdateTrailBuffer();
}
