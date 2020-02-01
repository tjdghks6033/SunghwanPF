#include "Framework.h"
#include "Trail.h"

Trail::Trail(Matrix * transform, UINT count)
	: world(transform), count(count)
{
	
	renderTarget = new RenderTarget(D3D::Get()->Width(), D3D::Get()->Height());
	depthStencil = new DepthStencil(D3D::Get()->Width(), D3D::Get()->Height());
	viewport = new Viewport((float)D3D::Get()->Width(), (float)D3D::Get()->Height());

	for (UINT i = 0; i < MAX_MODEL_TRAIL; i++)
		D3DXMatrixIdentity(&worlds[i]);

	instanceBuffer = new VertexBuffer(worlds, MAX_MODEL_TRAIL, sizeof(Matrix), 1, true);
	
	float w = 0.5f;
	float h = 0.5f;
	
	// Create Vertex Buffer
	{
		VertexTexture* vertices = new VertexTexture[6 * count];

		float dx = 1.0f / count; // ºñÀ²

		for (UINT i = 0; i < count; i++)
		{
			vertices[i * 6 + 0].Position = D3DXVECTOR3(-0.5f + (dx * i), -0.5f, 0.0f);
			vertices[i * 6 + 1].Position = D3DXVECTOR3(-0.5f + (dx * i), 0.5f, 0.0f);
			vertices[i * 6 + 2].Position = D3DXVECTOR3(-0.5f + dx * (i + 1), -0.5f, 0.0f);
			vertices[i * 6 + 3].Position = D3DXVECTOR3(-0.5f + dx * (i + 1), -0.5f, 0.0f);
			vertices[i * 6 + 4].Position = D3DXVECTOR3(-0.5f + (dx * i), 0.5f, 0.0f);
			vertices[i * 6 + 5].Position = D3DXVECTOR3(-0.5f + dx * (i + 1), 0.5f, 0.0f);

			vertices[i * 6 + 0].Uv = D3DXVECTOR2((dx * i), 1);
			vertices[i * 6 + 1].Uv = D3DXVECTOR2((dx * i), 0);
			vertices[i * 6 + 2].Uv = D3DXVECTOR2(dx * (i + 1), 1);
			vertices[i * 6 + 3].Uv = D3DXVECTOR2(dx * (i + 1), 1);
			vertices[i * 6 + 4].Uv = D3DXVECTOR2((dx * i), 0);
			vertices[i * 6 + 5].Uv = D3DXVECTOR2(dx * (i + 1), 0);

		}

		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTexture) * 6 * count;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));

		SafeDeleteArray(vertices);
	}

	
	texture = new Texture(L"White.png");
	shader = new Shader(L"48_Water.fxo");

	buffer = new ConstantBuffer(&desc, sizeof(Desc));
	sBuffer = shader->AsConstantBuffer("CB_Trail");
	sTrailMap = shader->AsSRV("TrailMap");

	//Create Sampler
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
		desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		desc.MaxAnisotropy = 1;
		desc.MaxLOD = FLT_MAX;

		Check(D3D::GetDevice()->CreateSamplerState(&desc, &samplerState));
		sSamplerState = shader->AsSampler("TrailSampler");
	}

	GenerateTrail();

}

Trail::~Trail()
{
	SafeDelete(buffer);

	SafeDelete(renderTarget);
	SafeDelete(depthStencil);
	SafeDelete(viewport);

	SafeRelease(samplerState);
}

void Trail::Update()
{
	UpdateTrailBuffer();
}

void Trail::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->DrawIndexed(0, pass, 6 * count);
}

void Trail::GenerateTrail()
{
	renderTarget->Set(depthStencil);
	viewport->RSSetViewport();

	buffer->Apply();
	sBuffer->SetConstantBuffer(buffer->Buffer());

	sTrailMap->SetResource(depthStencil->SRV());
	sSamplerState->SetSampler(0, samplerState);
}

void Trail::UpdateTrailBuffer()
{
	for (UINT i = 0; i < transforms.size(); i++)
		memcpy(worlds[i], transforms[i]->World(), sizeof(Matrix));

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(instanceBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, worlds, sizeof(Matrix) * MAX_MODEL_TRAIL);
	}
	D3D::GetDC()->Unmap(instanceBuffer->Buffer(), 0);
}

void Trail::UpdateTransform()
{
	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
	Vector3 translation = Vector3(0, 0, 0);
	Vector3 rotation = Vector3(0, 0, 0);

	D3DXMATRIX S, T, R;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&T, translation.x, translation.y, translation.z);
	D3DXMatrixRotationYawPitchRoll(&R, Math::ToRadian(rotation.y), Math::ToRadian(rotation.x), Math::ToRadian(rotation.z));
	worlds[0] = S * T * R;
}
