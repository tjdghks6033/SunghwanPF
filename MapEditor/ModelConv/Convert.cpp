#include "stdafx.h"
#include "Convert.h"
#include "Converter.h"

void Convert::Initialize()
{
	//Airplane();
	//Tower();
	//Tank();
	//Stones();
	//Tree();
	//Kachujin();
	//Megan();
	//Weapon();
	//Dreyar(); 
	CastleGuard();
}

void Convert::Airplane()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"B787/Airplane.fbx");
	conv->ExportMaterial(L"B787/Airplane", false);
	conv->ExportMesh(L"B787/Airplane");

	SafeDelete(conv);
}

void Convert::Tower()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Tower/Tower.fbx");
	conv->ExportMaterial(L"Tower/Tower", false);
	conv->ExportMesh(L"Tower/Tower");

	SafeDelete(conv);
}

void Convert::Tank()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Tank/Tank.fbx");
	conv->ExportMaterial(L"Tank/Tank", false);
	conv->ExportMesh(L"Tank/Tank");

	SafeDelete(conv);
}

void Convert::Stones()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Stones/stone_1.fbx");
	conv->ExportMaterial(L"Stones/stone_1", false);
	conv->ExportMesh(L"Stones/stone_1");
	SafeDelete(conv);

	Converter* conv2 = new Converter();
	conv2->ReadFile(L"Stones/stone_2.fbx");
	conv2->ExportMaterial(L"Stones/stone_2", false);
	conv2->ExportMesh(L"Stones/stone_2");
	SafeDelete(conv2);

	Converter* conv3 = new Converter();
	conv3->ReadFile(L"Stones/stone_3.fbx");
	conv3->ExportMaterial(L"Stones/stone_3", false);
	conv3->ExportMesh(L"Stones/stone_3");
	SafeDelete(conv3);

	Converter* conv4 = new Converter();
	conv4->ReadFile(L"Stones/stone_4.fbx");
	conv4->ExportMaterial(L"Stones/stone_4", false);
	conv4->ExportMesh(L"Stones/stone_4");
	SafeDelete(conv4);

	Converter* conv5 = new Converter();
	conv5->ReadFile(L"Stones/stone_5.fbx");
	conv5->ExportMaterial(L"Stones/stone_5", false);
	conv5->ExportMesh(L"Stones/stone_5");
	SafeDelete(conv5);
}
void Convert::Tree()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Tree/Tree1.obj");
	conv->ExportMaterial(L"Tree/Tree1", false);
	conv->ExportMesh(L"Tree/Tree1");
	SafeDelete(conv);

	Converter* conv2 = new Converter();
	conv2->ReadFile(L"Tree/Tree2.fbx");
	conv2->ExportMaterial(L"Tree/Tree2", false);
	conv2->ExportMesh(L"Tree/Tree2");
	SafeDelete(conv2);
	
	Converter* conv3 = new Converter();
	conv3->ReadFile(L"Tree/cgaxis_models_115_37_fbx.fbx");
	conv3->ExportMaterial(L"Tree/Tree3" , false);
	conv3->ExportMesh(L"Tree/Tree3");
	SafeDelete(conv3);

	Converter* conv4 = new Converter();
	conv4->ReadFile(L"Tree/Tree3.fbx");
	conv4->ExportMaterial(L"Tree/Tree4", false);
	conv4->ExportMesh(L"Tree/Tree4");
	SafeDelete(conv4);

	Converter* conv5 = new Converter();
	conv5->ReadFile(L"Tree/TREE_TRUNK_26_10K.fbx");
	conv5->ExportMaterial(L"Tree/Tree5", false);
	conv5->ExportMesh(L"Tree/Tree5");
	SafeDelete(conv5);
}
void Convert::Kachujin()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Kachujin/Mesh.fbx");
	conv->ExportMaterial(L"Kachujin/Mesh", false);
	conv->ExportMesh(L"Kachujin/Mesh");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Idle.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Idle");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Running.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Running");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Jump.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Jump");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Hip_Hop_Dancing.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Hip_Hop_Dancing");
	SafeDelete(conv);
}

void Convert::Dreyar()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Dreyar/Dreyar.fbx");
	conv->ExportMaterial(L"Dreyar/Mesh", false);
	conv->ExportMesh(L"Dreyar/Mesh");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/Idle.fbx");
	conv->ExportAnimClip(0, L"Dreyar/Idle");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/Running.fbx");
	conv->ExportAnimClip(0, L"Dreyar/Running");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/Attacking.fbx");
	conv->ExportAnimClip(0, L"Dreyar/Attacking");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/WarmingUp.fbx");
	conv->ExportAnimClip(0, L"Dreyar/WarmingUp");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/BoxingJab.fbx");
	conv->ExportAnimClip(0, L"Dreyar/BoxingJab");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/BoxingBodyJab.fbx");
	conv->ExportAnimClip(0, L"Dreyar/BoxingBodyJab");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/BoxingOneTwo.fbx");
	conv->ExportAnimClip(0, L"Dreyar/BoxingOneTwo");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/StandingReactLeft.fbx");
	conv->ExportAnimClip(0, L"Dreyar/StandingReactLeft");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/StandingReactLargeLeft.fbx");
	conv->ExportAnimClip(0, L"Dreyar/StandingReactLargeLeft");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/UnarmedEquipOverShoulder.fbx");
	conv->ExportAnimClip(0, L"Dreyar/UnarmedEquipOverShoulder");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/AttackCombo_1.fbx");
	conv->ExportAnimClip(0, L"Dreyar/AttackCombo_1");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/AttackCombo_2.fbx");
	conv->ExportAnimClip(0, L"Dreyar/AttackCombo_2");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/AttackCombo_3.fbx");
	conv->ExportAnimClip(0, L"Dreyar/AttackCombo_3");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/MagicAttack_1.fbx");
	conv->ExportAnimClip(0, L"Dreyar/MagicAttack_1");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/MagicAttack_2.fbx");
	conv->ExportAnimClip(0, L"Dreyar/MagicAttack_2");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/StandingDrawArrow.fbx");
	conv->ExportAnimClip(0, L"Dreyar/StandingDrawArrow");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/StandingAimRecoil.fbx");
	conv->ExportAnimClip(0, L"Dreyar/StandingAimRecoil");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/ShootingArrow.fbx");
	conv->ExportAnimClip(0, L"Dreyar/ShootingArrow");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Dreyar/StandingDeathRight.fbx");
	conv->ExportAnimClip(0, L"Dreyar/StandingDeathRight");
	SafeDelete(conv);
}

void Convert::CastleGuard()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"CastleGuard/CastleGuard.fbx");
	conv->ExportMaterial(L"CastleGuard/Mesh", false);
	conv->ExportMesh(L"CastleGuard/Mesh");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"CastleGuard/Idle.fbx");
	conv->ExportAnimClip(0, L"CastleGuard/Idle");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"CastleGuard/Running.fbx");
	conv->ExportAnimClip(0, L"CastleGuard/Running");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"CastleGuard/Attacking.fbx");
	conv->ExportAnimClip(0, L"CastleGuard/Attacking");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"CastleGuard/StandingReactLeft.fbx");
	conv->ExportAnimClip(0, L"CastleGuard/StandingReactLeft");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"CastleGuard/ShootingArrow.fbx");
	conv->ExportAnimClip(0, L"CastleGuard/ShootingArrow");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"CastleGuard/StandingDeathBackward.fbx");
	conv->ExportAnimClip(0, L"CastleGuard/StandingDeathBackward");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"CastleGuard/StandingReactRight.fbx");
	conv->ExportAnimClip(0, L"CastleGuard/StandingReactRight");
	SafeDelete(conv);
}

void Convert::Megan()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Megan/Mesh.fbx");
	conv->ExportMaterial(L"Megan/Mesh", false);
	conv->ExportMesh(L"Megan/Mesh");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Megan/Taunt.fbx");
	conv->ExportAnimClip(0, L"Megan/Taunt");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Megan/Dancing.fbx");
	conv->ExportAnimClip(0, L"Megan/Dancing");
	SafeDelete(conv);
}

void Convert::Weapon()
{
	vector<wstring> names;
	names.push_back(L"Cutter.fbx");
	names.push_back(L"Cutter2.fbx");
	names.push_back(L"Dagger_epic.fbx");
	names.push_back(L"Dagger_small.fbx");
	names.push_back(L"Katana.fbx");
	names.push_back(L"LongArrow.obj");
	names.push_back(L"LongBow.obj");
	names.push_back(L"Rapier.fbx");
	names.push_back(L"Sword.fbx");
	names.push_back(L"Sword2.fbx");
	names.push_back(L"Sword_epic.fbx");

	for (wstring name : names)
	{
		Converter* conv = new Converter();
		conv->ReadFile(L"Weapon/" + name);


		String::Replace(&name, L".fbx", L"");
		String::Replace(&name, L".obj", L"");

		conv->ExportMaterial(L"Weapon/" + name, false);
		conv->ExportMesh(L"Weapon/" + name);
		SafeDelete(conv);
	}
}
