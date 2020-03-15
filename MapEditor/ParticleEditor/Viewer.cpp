#include "stdafx.h"
#include "Viewer.h"
#include "Particle/ParticleSystem.h"

void Viewer::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	Context::Get()->GetCamera()->Position(0, 32, -67);
	((Freedom *)Context::Get()->GetCamera())->Speed(20, 2);


	shader = new Shader(L"48_Water.fxo");
	Mesh();

	particle = new ParticleSystem(L"Smoke");
	
}

void Viewer::Destroy()
{
	
}

void Viewer::Update()
{
	grid->Update();

	Vector3 position;
	sphere->GetTransform(0)->Position(&position);

	if (Keyboard::Get()->Press('L'))
		position.x += 20 * Time::Delta();
	else if (Keyboard::Get()->Press('J'))
		position.x -= 20 * Time::Delta();

	if (Keyboard::Get()->Press('I'))
		position.z += 20 * Time::Delta();
	else if (Keyboard::Get()->Press('K'))
		position.z -= 20 * Time::Delta();

	if (Keyboard::Get()->Press('O'))
		position.y += 20 * Time::Delta();
	else if (Keyboard::Get()->Press('U'))
		position.y -= 20 * Time::Delta();

	sphere->GetTransform(0)->Position(position);
	sphere->GetTransform(0)->Scale(3, 3, 3);
	sphere->UpdateTransforms();

	sky->Update();
	sphere->Update();
}

void Viewer::PreRender()
{
	
}

void Viewer::Render()
{
	sky->Render();
	floor->Render();

	grid->Pass(7);
	grid->Render();

	brick->Render();
	sphere->Pass(4);
	sphere->Render();

	particle->Render();

}

void Viewer::PostRender()
{
	
}

void Viewer::Mesh()
{
	//Create Material
	{
		floor = new Material(shader);
		floor->DiffuseMap("Floor.png");
		floor->SpecularMap("Floor_Specular.png");
		floor->NormalMap("Floor_Normal.png");
		floor->Specular(1, 1, 1, 20);
		floor->Emissive(0.15f, 0.15f, 0.15f, 0.3f);

		brick = new Material(shader);
		brick->DiffuseMap("Bricks.png");
		brick->SpecularMap("Bricks_Specular.png");
		brick->NormalMap("Bricks_Normal.png");
		brick->Specular(1, 0.3f, 0.3f, 20);
		brick->Emissive(0.15f, 0.15f, 0.15f, 0.3f);
	}

	//Create Mesh
	{
		Transform* transform = NULL;

		grid = new MeshRender(shader, new MeshGrid(15, 15));
		transform = grid->AddTransform();
		transform->Position(0, 0, 0);
		transform->Scale(20, 1, 20);

		sphere = new MeshRender(shader, new MeshSphere(0.5f, 20, 20));
		transform = sphere->AddTransform();
		transform->Position(0, 0, 0); 
		transform->Scale(5, 5, 5);
		
	}
	grid->UpdateTransforms();
	sphere->UpdateTransforms();
}