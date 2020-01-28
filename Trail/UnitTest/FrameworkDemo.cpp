#include "stdafx.h"
#include "FrameworkDemo.h"
#include "Viewer/Freedom.h"
#include "Environment/Terrain.h"

void FrameworkDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	Context::Get()->GetCamera()->Position(0, 32, -67);
	((Freedom *)Context::Get()->GetCamera())->Speed(20, 2);


	shader = new Shader(L"14_Mesh.fx");

	Transform* transform = NULL;

	cube = new MeshRender(shader, new MeshCube());
	transform = cube->AddTransform();
	transform->Position(0, 5, 0);
	transform->Scale(20, 10, 20);
	cube->UpdateTransforms();

	grid = new MeshRender(shader, new MeshGrid(5, 5));
	transform = grid->AddTransform();
	transform->Scale(20, 1, 20);
	grid->UpdateTransforms();

	cylinder = new MeshRender(shader, new MeshCylinder(0.5f, 3.0f, 20, 20));
	sphere = new MeshRender(shader, new MeshSphere(0.5f, 20, 20));
	for (UINT i = 0; i < 5; i++)
	{
		cylinder->AddTransform();
		cylinder->GetTransform(i)->Position(-30, 6, -15.0f + (float)i * 15.0f);
		cylinder->GetTransform(i)->Scale(5, 5, 5);

		cylinder->AddTransform();
		cylinder->GetTransform(i)->Position(30, 6, -15.0f + (float)i * 15.0f);
		cylinder->GetTransform(i)->Scale(5, 5, 5);


		sphere->AddTransform();
		sphere->GetTransform(i)->Position(-30, 15.5f, -15.0f + (float)i * 15.0f);
		sphere->GetTransform(i)->Scale(5, 5, 5);

		sphere->AddTransform();
		sphere->GetTransform(i)->Position(30, 15.5f, -15.0f + (float)i * 15.0f);
		sphere->GetTransform(i)->Scale(5, 5, 5);
	}
	cylinder->UpdateTransforms();
	sphere->UpdateTransforms();


	terrainShader = new Shader(L"11_Terrain.fx");
	terrain = new Terrain(terrainShader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt3.png");
}

void FrameworkDemo::Destroy()
{
	
}

void FrameworkDemo::Update()
{
	terrain->Update();
}

void FrameworkDemo::Render()
{
	cylinder->Render();
	sphere->Render();

	cube->Render();
	grid->Render();

	terrain->Render();
}