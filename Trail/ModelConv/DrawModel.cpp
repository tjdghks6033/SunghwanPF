#include "stdafx.h"
#include "DrawModel.h"
#include "Viewer/Freedom.h"

void DrawModel::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -50);
	((Freedom *)Context::Get()->GetCamera())->Speed(20, 2);

	shader = new Shader(L"27_Model.fxo");

	//Airplane();
	//Tower();
	//Tank();
	Castle();
}

void DrawModel::Update()
{
	if (airplane != NULL) airplane->Update();
	if (tower != NULL) tower->Update();
	if (tank != NULL) tank->Update();
	if (castle != NULL) castle->Update();
}

void DrawModel::Render()
{
	if (airplane != NULL) airplane->Render();
	if (tower != NULL) tower->Render();
	if (tank != NULL) tank->Render();
	if (castle != NULL) castle->Render();
}

void DrawModel::Airplane()
{
	airplane = new ModelRender(shader);
	airplane->ReadMaterial(L"B787/Airplane");
	airplane->ReadMesh(L"B787/Airplane");
	
	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = airplane->AddTransform();

		transform->Position(Vector3(x, 0.0f, 2.5f));
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
		transform->Scale(0.00025f, 0.00025f, 0.00025f);
	}
	airplane->UpdateTransforms();

	airplane->Pass(1);
}

void DrawModel::Tower()
{
	tower = new ModelRender(shader);
	tower->ReadMaterial(L"Tower/Tower");
	tower->ReadMesh(L"Tower/Tower");
	
	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = tower->AddTransform();

		transform->Position(Vector3(x, 0.0f, 7.5f));
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
		transform->Scale(0.003f, 0.003f, 0.003f);
	}
	tower->UpdateTransforms();

	tower->Pass(1);
}

void DrawModel::Tank()
{
	tank = new ModelRender(shader);
	tank->ReadMaterial(L"Tank/Tank");
	tank->ReadMesh(L"Tank/Tank");
	
	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = tank->AddTransform();

		transform->Position(Vector3(x, 0.0f, 5.0f));
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
		transform->Scale(0.1f, 0.1f, 0.1f);
	}
	tank->UpdateTransforms();

	tank->Pass(1);
}

void DrawModel::Castle()
{
	castle = new ModelRender(shader);
	castle->ReadMaterial(L"Castle/Mesh");
	castle->ReadMesh(L"Castle/Mesh");
	
	{
		Transform* transform = castle->AddTransform();

		transform->Position(Vector3(0, 0.0f, 5.0f));
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
		transform->Scale(1.0f, 1.0f, 1.0f);
	}
	castle->UpdateTransforms();

	castle->Pass(1);
}
