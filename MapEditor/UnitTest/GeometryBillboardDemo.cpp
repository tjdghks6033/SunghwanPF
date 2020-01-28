#include "stdafx.h"
#include "GeometryBillboardDemo.h"
#include "Viewer/Freedom.h"
#include "Environment/CubeSky.h"
#include "Environment/Terrain.h"
#include "Environment/Billboard.h"

void GeometryBillboardDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(11, 0, 0);
	Context::Get()->GetCamera()->Position(132, 42, -17);
	((Freedom *)Context::Get()->GetCamera())->Speed(100, 2);

	shader = new Shader(L"18_Terrain_Splatting.fx");

	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt3.png");
	terrain->LayerMap(L"Terrain/Grass (Hill).jpg", L"Terrain/Gray256.png");

	sky = new CubeSky(L"Environment/GrassCube1024.dds");


	bbShader = new Shader(L"25_Billboard.fx");
	bb = new Billboard(bbShader);
	bb->AddTexture(L"White.png");
	bb->AddTexture(L"Green.png");
	bb->AddTexture(L"Blue.png");
	bb->AddTexture(L"Red.png");
	bb->AddTexture(L"Magenta.png");
	bb->AddTexture(L"Yellow.png");

	for (UINT i = 0; i < 1000; i++)
	{
		Vector2 scale = Math::RandomVec2(3.0f, 10.0f);
		Vector3 position = Math::RandomVec3(0.0f, 256.0f);
		position.y = terrain->GetHeight(position) + scale.y * 0.5f;
		
		bb->Add(position, scale);
	}
}

void GeometryBillboardDemo::Destroy()
{
	
}

void GeometryBillboardDemo::Update()
{
	sky->Update();
	terrain->Update();
	bb->Update();
}

void GeometryBillboardDemo::Render()
{
	sky->Render();
	terrain->Render();
	bb->Render();
}