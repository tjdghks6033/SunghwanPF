#include "stdafx.h"
#include "TerrainSplattingDemo.h"
#include "Viewer/Freedom.h"
#include "Environment/Terrain.h"

void TerrainSplattingDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(11, 0, 0);
	Context::Get()->GetCamera()->Position(132, 42, -17);
	((Freedom *)Context::Get()->GetCamera())->Speed(100, 2);

	shader = new Shader(L"18_Terrain_Splatting.fx");

	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt3.png");
	terrain->LayerMap(L"Terrain/Grass (Hill).jpg", L"Terrain/Grayscale.png");
}

void TerrainSplattingDemo::Destroy()
{
	
}

void TerrainSplattingDemo::Update()
{
	terrain->Update();
}

void TerrainSplattingDemo::Render()
{
	terrain->Render();
}