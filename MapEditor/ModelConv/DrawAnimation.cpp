#include "stdafx.h"
#include "DrawAnimation.h"
#include "Viewer/Freedom.h"

void DrawAnimation::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(0, 5, -15);
	((Freedom *)Context::Get()->GetCamera())->Speed(20, 2);

	shader = new Shader(L"27_Animation.fxo");

	Kachujin();
	Dreyar();
	CastleGuard();
}

void DrawAnimation::Update()
{	
	castleGuard->Update();
	if (kachujin != NULL)
	{
		if (Keyboard::Get()->Down(VK_SHIFT))
		{
			++playerClip;

			//kachujin
			//clip %= 4;
			//kachujin->PlayClip(20, clip, 1.0f, 1.0f);

			playerClip %= 4;
			kachujin->PlayClip(0, playerClip, speed, taketime);
		}

		kachujin->Update();
	}
	if (dreyar != NULL)
	{
		if (Keyboard::Get()->Down(VK_SPACE))
		{
			++playerClip;
			playerClip %= 15;

			if(playerClip != 9)
				dreyar->PlayClip(0, playerClip, speed, taketime);
		}

		if (Keyboard::Get()->Down(VK_UP))
		{
			playerClip = 9;
			dreyar->PlayClip(0, playerClip, speed, taketime);
		}
		
		ImGui::SliderFloat("speed", &speed, 0, 2);
		ImGui::SliderFloat("taketime", &taketime, 0, 1);

		static int b = 22;
		ImGui::SliderInt("b", &b, 0, 50);

		if (playerClip == 9)
		{
			int a = dreyar->GetTime();
			/*if (a > 25)
				a = 0;*/
			ImGui::SliderInt("a", &a, 0, 100);
			if (a == b)
			{
				weapon_num++;
				weapon_num %= 3;
			}			
		}

		if (weapon_num == 1)
		{
			if (Keyboard::Get()->Down(VK_SHIFT))
			{
				playerClip = Math::Random(4, 6);

				dreyar->PlayClip(0, playerClip, speed, taketime);

			}
		}
		if (weapon_num == 2)
		{
			if (Keyboard::Get()->Down(VK_SHIFT))
			{
				playerClip = Math::Random(10, 12);

				dreyar->PlayClip(0, playerClip, speed, taketime);
			}
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
	castleGuard->Render();


	static bool is_unarmed = false;
	static bool is_sword_spine = false;
	static bool is_sword = false;

	if (dreyar != NULL)
	{
		dreyar->Pass(2);

		if (weapon_num == 0)
		{
			if (is_sword)
			{
				dreyar->GetModel()->Dettach(weapon, 36);
				
				is_sword = false;
			}
			dreyar->SetWeaponNum(0);
			dreyar->Render();

			is_unarmed = true;
		}
		else if (weapon_num == 1)
		{
			if (is_unarmed)
			{
				Transform attachspineTransform;
				attachspineTransform.Position(-90, 330, 200);
				attachspineTransform.Rotation(-1.7f, 0, 1.7f);
				attachspineTransform.Scale(10.0f, 10.0f, 10.0f);

				dreyar->GetModel()->Attach(shader, weapon, 3, &attachspineTransform);

				is_unarmed = false;
				is_sword_spine = true;

				playerClip = 3;
				dreyar->PlayClip(0, playerClip, speed, taketime);
			}
			dreyar->SetWeaponNum(1);

			dreyar->Render2();
		}
		else if (weapon_num == 2)
		{
			if (is_sword_spine)
			{
				dreyar->GetModel()->Dettach(weapon, 3);

				Transform attachTransform;
				attachTransform.Position(-60, -20, -200);
				attachTransform.Scale(10.0f, 10.0f, 10.0f);

				dreyar->GetModel()->Attach(shader, weapon, 36, &attachTransform);

				is_sword_spine = false;
				is_sword = true;

				playerClip = 0;
				dreyar->PlayClip(0, playerClip, speed, taketime);
			}
			dreyar->SetWeaponNum(2);
			dreyar->Render3();
		}
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
	dreyar->ReadClip(L"Dreyar/Idle");						//0
	dreyar->ReadClip(L"Dreyar/Running");					//1
	dreyar->ReadClip(L"Dreyar/Attacking");					//2
	dreyar->ReadClip(L"Dreyar/WarmingUp");					//3
	dreyar->ReadClip(L"Dreyar/BoxingJab");					//4
	dreyar->ReadClip(L"Dreyar/BoxingBodyJab");				//5
	dreyar->ReadClip(L"Dreyar/BoxingOneTwo");				//6
	dreyar->ReadClip(L"Dreyar/StandingReactLeft");			//7
	dreyar->ReadClip(L"Dreyar/StandingReactLargeLeft");		//8
	dreyar->ReadClip(L"Dreyar/UnarmedEquipOverShoulder");	//9
	dreyar->ReadClip(L"Dreyar/AttackCombo_1");				//10
	dreyar->ReadClip(L"Dreyar/AttackCombo_2");				//11
	dreyar->ReadClip(L"Dreyar/AttackCombo_3");				//12
	dreyar->ReadClip(L"Dreyar/MagicAttack_1");				//13
	dreyar->ReadClip(L"Dreyar/MagicAttack_2");				//14
	

	dreyar->SetWeaponNum(0);
	dreyar->Render();

	Transform attachspineTransform;
	attachspineTransform.Position(-90, 330, 200);
	attachspineTransform.Rotation(-1.7f, 0, 1.7f);
	attachspineTransform.Scale(10.0f, 10.0f, 10.0f);

	dreyar->SetWeaponNum(1);
	dreyar->GetModel()->Attach(shader, weapon, 3, &attachspineTransform);
	dreyar->Render2();

	Transform attachTransform;
	attachTransform.Position(-60, -20, -200);
	attachTransform.Scale(10.0f, 10.0f, 10.0f);

	dreyar->SetWeaponNum(2);
	dreyar->GetModel()->Dettach(weapon, 3);
	dreyar->GetModel()->Attach(shader, weapon, 36, &attachTransform);
	dreyar->Render3();

	dreyar->GetModel()->Dettach(weapon, 36);
	dreyar->SetWeaponNum(0);

	dreyar->Pass(2);

	Transform* transform = dreyar->AddTransform();
	transform->Position(6, 0, 0);
	transform->Scale(0.001f, 0.001f, 0.001f);

	dreyar->UpdateTransforms();
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
	attachTransform.Position(-10, 0, -10);
	attachTransform.Scale(1.0f, 1.0f, 1.0f);

	castleGuard->GetModel()->Attach(shader, weapon, 23, &attachTransform);

	castleGuard->Pass(2);

	Transform* transform = castleGuard->AddTransform();
	transform->Position(4, 0, 0);
	transform->Scale(0.01f, 0.01f, 0.01f);

	castleGuard->UpdateTransforms();
}