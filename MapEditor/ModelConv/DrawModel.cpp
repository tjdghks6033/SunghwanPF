#include "stdafx.h"
#include "DrawModel.h"
#include "Viewer/Freedom.h"

void DrawModel::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(-7, -187, 0);
	Context::Get()->GetCamera()->Position(19, 7, 73);
	((Freedom *)Context::Get()->GetCamera())->Speed(20, 2);

	shader = new Shader(L"27_Model.fx");

	//Airplane();
	//Tower();
	//Tank();
	Stones();
	Tree();
}

void DrawModel::Update()
{
	if (airplane != NULL) airplane->Update();
	if (tower != NULL) tower->Update();
	if (tank != NULL) tank->Update();
	if (stone1 != NULL) stone1->Update();
	if (stone2 != NULL) stone2->Update();
	if (stone3 != NULL) stone3->Update();
	if (stone4 != NULL) stone4->Update();
	if (stone5 != NULL) stone5->Update();
	if (tree1 != NULL) tree1->Update();
	if (tree2 != NULL) tree2->Update();
	if (tree3 != NULL) tree3->Update();
	if (tree4 != NULL) tree4->Update();
	if (tree4 != NULL) tree5->Update();
}

void DrawModel::Render()
{
	if (airplane != NULL) airplane->Render();
	if (tower != NULL) tower->Render();
	if (tank != NULL) tank->Render();
	if (stone1 != NULL) stone1->Render();
	if (stone2 != NULL) stone2->Render();
	if (stone3 != NULL) stone3->Render();
	if (stone4 != NULL) stone4->Render();
	if (stone5 != NULL) stone5->Render();
	if (tree1 != NULL) tree1->Render();
	if (tree2 != NULL) tree2->Render();
	if (tree3 != NULL) tree3->Render();
	if (tree4 != NULL) tree4->Render();
	if (tree4 != NULL) tree5->Render();
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

void DrawModel::Stones()
{
	stone1 = new ModelRender(shader);
	stone1->ReadMaterial(L"Stones/stone_1");
	stone1->ReadMesh(L"Stones/stone_1");

	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = stone1->AddTransform();

		transform->Position(Vector3(x, 10.0f, 5.0f));
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
		transform->Scale(0.1f, 0.1f, 0.1f);
	}
	stone1->UpdateTransforms();

	stone1->Pass(1);

	stone2 = new ModelRender(shader);
	stone2->ReadMaterial(L"Stones/stone_2");
	stone2->ReadMesh(L"Stones/stone_2");

	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = stone2->AddTransform();

		transform->Position(Vector3(x, 20.5f, 5.0f));
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
		transform->Scale(0.1f, 0.1f, 0.1f);
	}
	stone2->UpdateTransforms();

	stone2->Pass(1);

	stone3 = new ModelRender(shader);
	stone3->ReadMaterial(L"Stones/stone_3");
	stone3->ReadMesh(L"Stones/stone_3");

	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = stone3->AddTransform();

		transform->Position(Vector3(x, 30.0f, 5.0f));
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
		transform->Scale(0.01f, 0.01f, 0.01f);
	}
	stone3->UpdateTransforms();

	stone3->Pass(1);

	stone4 = new ModelRender(shader);
	stone4->ReadMaterial(L"Stones/stone_4");
	stone4->ReadMesh(L"Stones/stone_4");

	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = stone4->AddTransform();

		transform->Position(Vector3(x, 40.0f, 5.0f));
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
		transform->Scale(0.01f, 0.01f, 0.01f);
	}
	stone4->UpdateTransforms();

	stone4->Pass(1);

	stone5 = new ModelRender(shader);
	stone5->ReadMaterial(L"Stones/stone_5");
	stone5->ReadMesh(L"Stones/stone_5");

	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = stone5->AddTransform();

		transform->Position(Vector3(x, 50.0f, 5.0f));
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
		transform->Scale(0.01f, 0.01f, 0.01f);
	}
	stone5->UpdateTransforms();

	stone5->Pass(1);
}

void DrawModel::Tree()
{
	tree1 = new ModelRender(shader);
	tree1->ReadMaterial(L"Tree/Tree1");
	tree1->ReadMesh(L"Tree/Tree1");
	//for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = tree1->AddTransform();

		transform->Position(Vector3(0, 0.0f, 5.0f));
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
		transform->Scale(1.0f, 1.0f, 1.0f);
	}
	tree1->UpdateTransforms();
	tree1->Pass(1);

	tree2 = new ModelRender(shader);
	tree2->ReadMaterial(L"Tree/Tree2");
	tree2->ReadMesh(L"Tree/Tree2");
	//for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = tree2->AddTransform();

		transform->Position(Vector3(10, 0.0f, 5.0f));
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
		transform->Scale(1.0f, 1.0f, 1.0f);
	}
	tree2->UpdateTransforms();
	tree2->Pass(1);

	tree3 = new ModelRender(shader);
	tree3->ReadMaterial(L"Tree/Tree3");
	tree3->ReadMesh(L"Tree/Tree3");
	//for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = tree3->AddTransform();

		transform->Position(Vector3(20, 0.0f, 5.0f));
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
		transform->Scale(1.0f, 1.0f, 1.0f);
	}
	tree3->UpdateTransforms();
	tree3->Pass(1);

	tree4 = new ModelRender(shader);
	tree4->ReadMaterial(L"Tree/Tree4");
	tree4->ReadMesh(L"Tree/Tree4");
	//for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = tree4->AddTransform();

		transform->Position(Vector3(30, 0.0f, 5.0f));
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
		transform->Scale(1.0f, 1.0f, 1.0f);
	}
	tree4->UpdateTransforms();
	tree4->Pass(1);

	tree5 = new ModelRender(shader);
	tree5->ReadMaterial(L"Tree/Tree5");
	tree5->ReadMesh(L"Tree/Tree5");
	//for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = tree5->AddTransform();

		transform->Position(Vector3(30, 0.0f, 5.0f));
		transform->RotationDegree(0, Math::Random(-180.0f, 180.0f), 0);
		transform->Scale(0.1f, 0.1f, 0.1f);
	}
	tree5->UpdateTransforms();
	tree5->Pass(1);
}