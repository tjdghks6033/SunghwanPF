#include "stdafx.h"
#include "TerrainDemo.h"
#include "Environment/Terrain.h"

void TerrainDemo::Initialize()
{
	shader = new Shader(L"08_Terrain.fx");

	terrain = new Terrain(shader, L"Terrain/Gray256.png");
}

void TerrainDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(terrain);
}

void TerrainDemo::Update()
{
	static Vector3 light = Vector3(-1, -1, 1);
	ImGui::SliderFloat3("Light", light, -1, 1);

	shader->AsVector("LightDirection")->SetFloatVector(light);

	terrain->Update();
}

void TerrainDemo::Render()
{
	terrain->Render();
}