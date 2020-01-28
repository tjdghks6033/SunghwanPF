#include "stdafx.h"
#include "TerrainLineDemo.h"
#include "Viewer/Freedom.h"
#include "Environment/Terrain.h"

void TerrainLineDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(11, 0, 0);
	Context::Get()->GetCamera()->Position(132, 42, -17);
	((Freedom *)Context::Get()->GetCamera())->Speed(100, 2);

	shader = new Shader(L"17_Terrain_Line.fx");

	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt3.png");
}

void TerrainLineDemo::Destroy()
{
	
}

void TerrainLineDemo::Update()
{
	terrain->Update();
}

void TerrainLineDemo::Render()
{
	terrain->Render();
}