#include "stdafx.h"
#include "WeatherDemo.h"
#include "Viewer/Freedom.h"
#include "Environment/CubeSky.h"
#include "Environment/Terrain.h"
#include "Environment/Billboard.h"
#include "Environment/Rain.h"
#include "Environment/Snow.h"

void WeatherDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(11, 0, 0);
	Context::Get()->GetCamera()->Position(132, 42, -17);
	((Freedom *)Context::Get()->GetCamera())->Speed(100, 2);

	shader = new Shader(L"18_Terrain_Splatting.fx");

	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt3.png");
	terrain->LayerMap(L"Terrain/Grass (Hill).jpg", L"Terrain/Gray256.png");

	sky = new CubeSky(L"Environment/GrassCube1024.dds");


	//bbShader = new Shader(L"23_Billboard.fx");
	//for (UINT i = 0; i < 1000; i++)
	//{
	//	bb[i] = new Billboard(bbShader, L"Terrain/Tree.png");


	//	Vector2 scale = Math::RandomVec2(3.0f, 10.0f);
	//	bb[i]->GetTransform()->Scale(scale.x, scale.y, 1.0f);

	//	Vector3 position = Math::RandomVec3(0.0f, 256.0f);
	//	position.y = terrain->GetHeight(position) + scale.y * 0.5f;
	//	bb[i]->GetTransform()->Position(position);
	//}

	rain = new Rain(Vector3(300, 100, 500), 10000, L"Environment/Rain.png");
	snow = new Snow(Vector3(300, 100, 500), 100000, L"Environment/Snow.png");
}

void WeatherDemo::Destroy()
{
	
}

void WeatherDemo::Update()
{
	sky->Update();
	terrain->Update();

	//for (UINT i = 0; i < 1000; i++)
	//	bb[i]->Update();

	
	UINT selected = (UINT)weather;

	ImGui::Separator();
	ImGui::InputInt("Weather", (int *)&selected);
	selected %= 3;
	weather = (Weather)selected;


	switch (weather)
	{
		case Weather::Rain: rain->Update(); break;
		case Weather::Snow: snow->Update(); break;
	}
}

void WeatherDemo::Render()
{
	sky->Render();
	terrain->Render();

	//for (UINT i = 0; i < 1000; i++)
	//	bb[i]->Render();

	
	switch (weather)
	{
		case Weather::Rain: rain->Render(); break;
		case Weather::Snow: snow->Render(); break;
	}
}