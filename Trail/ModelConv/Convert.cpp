#include "stdafx.h"
#include "Convert.h"
#include "Converter.h"

void Convert::Initialize()
{
	//Airplane();
	//Tower();
	//Tank();
	//Megan();
	//Weapon();
	//Kachujin();
	//Amy();
	//CastleGuard();
	//Dreyar();
	//Ganfaul();
	Castle();
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
	conv->ReadFile(L"Kachujin/Standing_Idle.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Standing_Idle");
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

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Attacking.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Attacking");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Taking Item.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Taking Item");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Standing_Aim_Recoil.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Standing_Aim_Recoil");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Standing_Hit_React.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Standing_Hit_React");
	SafeDelete(conv);
	conv = new Converter();
	conv->ReadFile(L"Kachujin/Standing_Hit_React_Right.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Standing_Hit_React_Right");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Standing_Death.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Standing_Death");
	SafeDelete(conv);
	conv = new Converter();
	conv->ReadFile(L"Kachujin/Standing_Death_Left.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Standing_Death_Left");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Big_Jump.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Big_Jump");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Standing_Dodge_Backward.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Standing_Dodge_Backward");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Sword_Attack.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Sword_Attack");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Sword_Jump_Attack.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Sword_Jump_Attack");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Kachujin/Dying.fbx");
	conv->ExportAnimClip(0, L"Kachujin/Dying");
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

void Convert::Amy()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Amy/Amy.fbx");
	conv->ExportMaterial(L"Amy/Mesh", false);
	conv->ExportMesh(L"Amy/Mesh");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Amy/Idle.fbx");
	conv->ExportAnimClip(0, L"Amy/Idle");
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
	conv->ReadFile(L"Kachujin/Idle.fbx");
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
}

void Convert::Ganfaul()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Ganfaul/Ganfaul.fbx");
	conv->ExportMaterial(L"Ganfaul/Mesh", false);
	conv->ExportMesh(L"Ganfaul/Mesh");
	SafeDelete(conv);

	conv = new Converter();
	conv->ReadFile(L"Ganfaul/Idle.fbx");
	conv->ExportAnimClip(0, L"Ganfaul/Idle");
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

void Convert::Castle()
{
	Converter* conv = new Converter();
	conv->ReadFile(L"Castle/Castle.fbx");
	conv->ExportMaterial(L"Castle/Mesh", false);
	conv->ExportMesh(L"Castle/Mesh");
	SafeDelete(conv);
}
