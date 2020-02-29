#include "Framework.h"
#include "Trail.h"

Trail::Trail(InitializeDesc & initDesc, UINT count, UINT dx)
	: Renderer(L"48_Water.fxo"), desc(initDesc), count(count), dx(dx)
{	
	float w = 0.5f;
	float h = 0.5f;

	float dxx = 1.0f / 64.0f; // ºñÀ²

	UINT i = dx;


	VertexTexture vertices[6];

	vertices[0].Position = Vector3(-1.0f, -1.0f, 0.0f);
	vertices[1].Position = Vector3(-1.0f, +1.0f, 0.0f);
	vertices[2].Position = Vector3(+1.0f, -1.0f, 0.0f);
	vertices[3].Position = Vector3(+1.0f, -1.0f, 0.0f);
	vertices[4].Position = Vector3(-1.0f, +1.0f, 0.0f);
	vertices[5].Position = Vector3(+1.0f, +1.0f, 0.0f);

	vertices[0].Uv = Vector2(0, 1);
	vertices[1].Uv = Vector2(0, 0);
	vertices[2].Uv = Vector2(1, 1);
	vertices[3].Uv = Vector2(1, 1);
	vertices[4].Uv = Vector2(0, 0);
	vertices[5].Uv = Vector2(1, 0);

	vertexBuffer = new VertexBuffer(vertices, 6, sizeof(VertexTexture));

	texture = new Texture(L"grr.png");

	buffer = new ConstantBuffer(&desc, sizeof(InitializeDesc));
	sBuffer = shader->AsConstantBuffer("CB_Trail");
	sTrailMap = shader->AsSRV("TrailMap");

}

Trail::~Trail()
{	
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
	Vector3 scaledecom;
	Math::MatrixDecompose(mat, scaledecom, rotatdecom, transdecom);

	transform->World(mat);
	transform->Position(transdecom.x + 0.4f, transdecom.y, transdecom.z - 0.3f);
	transform->Scale(Vector3(0.01f, 0.5f, 0.5f));
	transform->Rotation(rotatdecom.x + 3.14f, rotatdecom.y - 1.57f, rotatdecom.z - 2.526f);

/*
	transform->Position(Vector3(transdecom.x , transdecom.y, transdecom.z));
	transform->Scale(Vector3(0.5f, 0.5f, 0.5f));
	transform->Rotation(Vector3(rotatdecom.x, rotatdecom.y, rotatdecom.z));*/
/*
	transform->Position(&transdecom);
	transform->Scale(&scaledecom);
	transform->Rotation(&rotatdecom);

	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, scaledecom.x, scaledecom.y, scaledecom.z);
	D3DXMatrixRotationYawPitchRoll(&R, Math::ToRadian(rotatdecom.y), Math::ToRadian(rotatdecom.x), Math::ToRadian(rotatdecom.z));
	D3DXMatrixTranslation(&T, transdecom.x, transdecom.y, transdecom.z);
	desc.trail = S * R * T;*/
}
