#include "stdafx.h"
#include "TessDistanceDemo.h"

void TessDistanceDemo::Initialize()
{
	shader = new Shader(L"41_TessDistance.fx");

	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -5);
	((Freedom *)Context::Get()->GetCamera())->Speed(10, 0);


	Vertex vertices[4];
	vertices[0].Position = Vector3(-10.0f, -10.0f, 0);
	vertices[1].Position = Vector3(-10.0f, +10.0f, 0);
	vertices[2].Position = Vector3(+10.0f, -10.0f, 0);
	vertices[3].Position = Vector3(+10.0f, +10.0f, 0);

	vertexBuffer = new VertexBuffer(vertices, 4, sizeof(Vertex));
	perFrame = new PerFrame(shader);
	transform = new Transform(shader);
}

void TessDistanceDemo::Update()
{
	static Vector2 distance = Vector2(1, 100);
	ImGui::SliderFloat2("Distance", distance, 0, 1000);

	shader->AsVector("Distance")->SetFloatVector(distance);

	transform->Update();
	perFrame->Update();
}

void TessDistanceDemo::Render()
{
	vertexBuffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	transform->Render();
	perFrame->Render();

	static UINT pass = 0;
	ImGui::InputInt("Pass", (int *)&pass);
	pass %= 4;

	shader->Draw(0, pass, 4);
}