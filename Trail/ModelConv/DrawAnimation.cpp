#include "stdafx.h"
#include "DrawAnimation.h"
#include "Viewer/Freedom.h"
#include "Model/Trail.h"

void DrawAnimation::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(0, 5, -15);
	((Freedom *)Context::Get()->GetCamera())->Speed(20, 2);

	shader = new Shader(L"27_Animation.fx");

	Kachujin();
	Amy();
	CastleGuard();
	Dreyar();
	Ganfaul();
}

void DrawAnimation::Update()
{
	static UINT clip = 0;

	if (kachujin != NULL)
	{
		if (Keyboard::Get()->Down(VK_SPACE))
		{
			++clip;
			/*clip %= 16;
			kachujin->PlayClip(0, clip, 1.0f, 1.0f);

			clip %= 4;
			castleGuard->PlayClip(0, clip, 1.0f, 1.0f);*/

			clip %= 11;
			dreyar->PlayClip(0, clip, 2.0f, 1.0f);
		}

		if (Keyboard::Get()->Down(VK_SHIFT))
		{			
			kachujin->PlayClip(0, 3, 1.0f, 1.0f);
		}
		kachujin->Update();

		//Matrix mmm = kachujin->GetAttachTransform(0);
		//trailor[0]->Update(mmm);
		
		/*Vector3 position;
		Vector3 scale;
		Vector3 rotation;

		trailor[0]->GetTransform()->Position(&position);
		trailor[0]->GetTransform()->Scale(&scale);
		trailor[0]->GetTransform()->Rotation(&rotation);

		ImGui::SliderFloat3("position", position2, -10.0f, 10.0f);
		ImGui::SliderFloat3("scale", scale2, -10.0f, 1.0f);
		ImGui::SliderFloat3("rotation", rotation2, -10.0f, 10.0f);
		
		trailor[0]->GetTransform()->World(mmm);
		
		trailor[0]->GetTransform()->Position(position + position2);
		trailor[0]->GetTransform()->Scale(scale + scale2);
		trailor[0]->GetTransform()->Rotation(rotation + rotation2);		

*/


		//trailor[0]->SetMatrix(mmm);

		/*for (int i = 1; i < 64; i++)
		{
			Matrix mmmm = trailor[i - 1]->GetMatrix();
			mmmm._41 -= 0.01f;
			trailor[i]->Update(mmmm);
			trailor[i]->SetMatrix(mmmm);
		}*/
	}
	if (amy != NULL) amy->Update();
	if (castleGuard != NULL) castleGuard->Update();
	if (dreyar != NULL) dreyar->Update();
	if (ganfaul != NULL) ganfaul->Update();

	for (int i = 0; i < 4; i++)
	{
		Matrix attach = kachujin->GetAttachTransform(0);
		trailor[0]->GetTransform()->World(attach);
		trailor[0]->Update(attach);
		
		colliders[i].Collider->GetTransform()->World(attach);
		colliders[i].Collider->Update();
	}
}

void DrawAnimation::Render()
{
	if (kachujin != NULL)
	{
		kachujin->Pass(2);
		kachujin->Render();
		for (int i = 0; i < 64; i++)
		{
			trailor[i]->Render();
		}
	}
	if (amy != NULL) amy->Render();
	if (castleGuard != NULL) castleGuard->Render();
	if (dreyar != NULL) dreyar->Render();
	if (ganfaul != NULL) ganfaul->Render();

	for (int i = 0; i < 4; i++)
		colliders[i].Collider->Render(Color(0, 1, 0, 1));
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
	kachujin->ReadClip(L"Kachujin/Dying");

	weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Sword");
	weapon->ReadMesh(L"Weapon/Sword");


	Transform attachTransform;
	attachTransform.Position(-10, 0, -10);
	attachTransform.Scale(1.0f, 1.0f, 1.0f);

	kachujin->GetModel()->Attach(shader, weapon, 35, &attachTransform);
	
	kachujin->Pass(2);

	Transform* transform = kachujin->AddTransform();
	transform->Position(0, 0, 0);
	transform->Scale(0.01f, 0.01f, 0.01f);

	Matrix mm = kachujin->GetAttachTransform(0);


	Trail::InitializeDesc desc = { mm };

	for (int i = 0; i < 64; i++)
	{
		desc.trail._41 -= 0.1f;
		trailor[i] = new Trail(desc, 1, i);
		trailor[i]->Pass(17);
	}

	/*for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = kachujin->AddTransform();
		transform->Position(x, 0, -5);
		transform->Scale(0.01f, 0.01f, 0.01f);
	}*/

	for (UINT i = 0; i < 4; i++)
	{
		colliders[i].Init = new Transform();
		colliders[i].Init->Scale(10, 10, 120);
		colliders[i].Init->Position(-10, 0, -60);

		colliders[i].Transform = new Transform();
		colliders[i].Collider = new Collider(colliders[i].Transform, colliders[i].Init);
	}

	kachujin->UpdateTransforms();	
}

void DrawAnimation::Amy()
{
	amy = new ModelAnimator(shader);
	amy->ReadMaterial(L"Amy/Mesh");
	amy->ReadMesh(L"Amy/Mesh");
	amy->ReadClip(L"Amy/Idle");
	
	Transform attachTransform;
	attachTransform.Position(-10, 0, -10);
	attachTransform.Scale(1.0f, 1.0f, 1.0f);

	amy->GetModel()->Attach(shader, weapon, 108, &attachTransform);

	amy->Pass(2);

	Transform* transform1 = amy->AddTransform();
	transform1->Position(2, 0, 0);
	transform1->Scale(0.01f, 0.01f, 0.01f);

	amy->UpdateTransforms();	
}

void DrawAnimation::CastleGuard()
{
	castleGuard = new ModelAnimator(shader);
	castleGuard->ReadMaterial(L"CastleGuard/Mesh");
	castleGuard->ReadMesh(L"CastleGuard/Mesh");
	castleGuard->ReadClip(L"CastleGuard/Idle");
	castleGuard->ReadClip(L"CastleGuard/Running");
	castleGuard->ReadClip(L"CastleGuard/Attacking");
	castleGuard->ReadClip(L"CastleGuard/StandingReactLeft");
	
	Transform attachTransform;
	attachTransform.Position(-20, 0, -25);
	attachTransform.Scale(1.0f, 1.0f, 1.0f);

	castleGuard->GetModel()->Attach(shader, weapon, 23, &attachTransform);

	castleGuard->Pass(2);

	Transform* transform = castleGuard->AddTransform();
	transform->Position(4, 0, 0);
	transform->Scale(0.01f, 0.01f, 0.01f);

	castleGuard->UpdateTransforms();
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

void DrawAnimation::Ganfaul()
{
	ganfaul = new ModelAnimator(shader);
	ganfaul->ReadMaterial(L"Ganfaul/Mesh");
	ganfaul->ReadMesh(L"Ganfaul/Mesh");
	ganfaul->ReadClip(L"Ganfaul/Idle");


	Transform attachTransform;
	attachTransform.Position(-10, 0, -10);
	attachTransform.Scale(1.0f, 1.0f, 1.0f);

	ganfaul->GetModel()->Attach(shader, weapon, 35, &attachTransform);

	ganfaul->Pass(2);

	Transform* transform = ganfaul->AddTransform();
	transform->Position(8, 0, 0);
	transform->Scale(0.01f, 0.01f, 0.01f);

	ganfaul->UpdateTransforms();
}
