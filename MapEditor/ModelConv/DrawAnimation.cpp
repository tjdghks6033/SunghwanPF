#include "stdafx.h"
#include "DrawAnimation.h"
#include "Viewer/Freedom.h"

void DrawAnimation::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(0, 5, -15);
	((Freedom *)Context::Get()->GetCamera())->Speed(20, 2);

	shader = new Shader(L"27_Animation.fx");

	Kachujin();
	Dreyar();
}

void DrawAnimation::Update()
{
	static UINT clip = 0;

	if (kachujin != NULL)
	{
		if (Keyboard::Get()->Down(VK_SPACE))
		{
			++clip;

			//kachujin
			//clip %= 4;
			//kachujin->PlayClip(20, clip, 1.0f, 1.0f);

			clip %= 11;
			dreyar->PlayClip(0, clip, 2.0f, 1.0f);
		}

		kachujin->Update();
	}

	if (dreyar != NULL)
	{
		if (Keyboard::Get()->Down(VK_SPACE))
		{
			clip %= 11;
			dreyar->PlayClip(0, clip, 2.0f, 1.0f);			
		}	
		dreyar->Update();
	}	
}

void DrawAnimation::Render()
{
	if (kachujin != NULL)
	{
		kachujin->Pass(2);
		kachujin->Render();
	}
	if (dreyar != NULL)
	{
		dreyar->Pass(2);
		dreyar->Render();
	}
}

void DrawAnimation::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Running");
	kachujin->ReadClip(L"Kachujin/Jump");
	kachujin->ReadClip(L"Kachujin/Hip_Hop_Dancing");

	weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Sword");
	weapon->ReadMesh(L"Weapon/Sword");


	Transform attachTransform;
	attachTransform.Position(-10, 0, -10);
	attachTransform.Scale(1.0f, 1.0f, 1.0f);

	kachujin->GetModel()->Attach(shader, weapon, 35, &attachTransform);
	kachujin->Pass(2);

	
	{
		Transform* transform = kachujin->AddTransform();
		transform->Position(0, 0, -5);
		transform->Scale(0.01f, 0.01f, 0.01f);
	}
	kachujin->UpdateTransforms();
}


void DrawAnimation::Dreyar()
{
	dreyar = new ModelAnimator(shader);
	dreyar->ReadMaterial(L"Dreyar/Mesh");
	dreyar->ReadMesh(L"Dreyar/Mesh");
	dreyar->ReadClip(L"Dreyar/Idle");
	dreyar->ReadClip(L"Dreyar/Running");
	dreyar->ReadClip(L"Dreyar/Attacking");
	dreyar->ReadClip(L"Dreyar/WarmingUp");
	dreyar->ReadClip(L"Dreyar/BoxingJab");
	dreyar->ReadClip(L"Dreyar/BoxingBodyJab");
	dreyar->ReadClip(L"Dreyar/BoxingOneTwo");
	dreyar->ReadClip(L"Dreyar/StandingReactLeft");
	dreyar->ReadClip(L"Dreyar/StandingReactLargeLeft");
	dreyar->ReadClip(L"Dreyar/UnarmedEquipOverShoulder");

	Transform attachTransform;
	attachTransform.Position(-140, 0, -140);
	attachTransform.Scale(10.0f, 10.0f, 10.0f);

	dreyar->GetModel()->Attach(shader, weapon, 36, &attachTransform);

	dreyar->Pass(2);

	Transform* transform = dreyar->AddTransform();
	transform->Position(6, 0, 0);
	transform->Scale(0.001f, 0.001f, 0.001f);

	dreyar->UpdateTransforms();
}
