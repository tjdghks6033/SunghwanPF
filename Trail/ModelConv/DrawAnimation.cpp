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
}

void DrawAnimation::Update()
{
	static UINT clip = 0;

	if (kachujin != NULL)
	{
		if (Keyboard::Get()->Down(VK_SPACE))
		{
			++clip;
			clip %= 16;
			kachujin->PlayClip(0, clip, 1.0f, 1.0f);
		}

		if (Keyboard::Get()->Down(VK_SHIFT))
		{			
			kachujin->PlayClip(0, 3, 1.0f, 1.0f);
		}

		kachujin->Update();
		trail->Update();
	}
}

void DrawAnimation::Render()
{
	if (kachujin != NULL)
	{
		kachujin->Pass(2);
		kachujin->Render();
		trail->Render();
	}
}

void DrawAnimation::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Standing_Idle");
	kachujin->ReadClip(L"Kachujin/Running");
	kachujin->ReadClip(L"Kachujin/Jump");
	kachujin->ReadClip(L"Kachujin/Hip_Hop_Dancing");
	kachujin->ReadClip(L"Kachujin/Attacking");
	kachujin->ReadClip(L"Kachujin/Standing_Aim_Recoil");
	kachujin->ReadClip(L"Kachujin/Standing_Hit_React");
	kachujin->ReadClip(L"Kachujin/Standing_Hit_React_Right");
	kachujin->ReadClip(L"Kachujin/Standing_Death");
	kachujin->ReadClip(L"Kachujin/Standing_Death_Left");
	kachujin->ReadClip(L"Kachujin/Big_Jump");
	kachujin->ReadClip(L"Kachujin/Standing_Dodge_Backward");
	kachujin->ReadClip(L"Kachujin/Sword_Attack");
	kachujin->ReadClip(L"Kachujin/Sword_Jump_Attack");

	weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Sword");
	weapon->ReadMesh(L"Weapon/Sword");


	Transform attachTransform;
	attachTransform.Position(-10, 0, -10);
	attachTransform.Scale(1.0f, 1.0f, 1.0f);

	kachujin->GetModel()->Attach(shader, weapon, 35, &attachTransform);
	kachujin->Pass(2);

	Transform* transform = kachujin->AddTransform();
	transform->Position(0, 0, -5);
	transform->Scale(0.01f, 0.01f, 0.01f);

	D3DXMATRIX weapon_transform = kachujin->GetModel()->BoneByIndex(35)->Transform();

	Trail trail(&weapon_transform, 64);
	Context::Get()->AddTrail(trail);
	trail->Pass(17);
	
	
	/*for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = kachujin->AddTransform();
		transform->Position(x, 0, -5);
		transform->Scale(0.01f, 0.01f, 0.01f);
	}*/
	kachujin->UpdateTransforms();
}