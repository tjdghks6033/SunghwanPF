#include "stdafx.h"
#include "WaterDemo.h"

void WaterDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(29, 2, 0);
	Context::Get()->GetCamera()->Position(124, 49, 10);
	((Freedom *)Context::Get()->GetCamera())->Speed(20, 2);


	shader = new Shader(L"48_Water.fxo");

	shadow = new Shadow(shader, Vector3(0, 0, 0), 65);

	//Sky
	{
		sky = new Sky(shader);
		sky->ScatteringPass(3);
		sky->RealTime(false, Math::PI - 1e-6f);
		//sky->RealTime(true, Math::PI - 1e-6f, 0.25f);v
	}
	//Weather
	{
		rain = new Rain(Vector3(300, 100, 500), 10000, L"Environment/Rain.png");
		snow = new Snow(Vector3(300, 300, 300), 10000, L"Environment/Snow.png");
	}

	//Water
	{	
		water = new Water(shader, 127.5);
		water->GetTransform()->Position(128.0f + 0, 5, 128.0f + 0);
		//water->GetTransform()->Position(0, 0.1f, 0);
	}
	//Ocean
	{
		sOcean = new Shader(L"50_OceanRendering.fxo");

		FFTOcean::InitializeInfo desc =
		{
			sOcean,
			L"Terrain/Gray512.png",
			1.0f,
			16,
		};

		ocean = new FFTOcean(desc);
	}

	//Terrain	
	{
		sTerrain = new Shader(L"18_Terrain_Splatting.fxo");

		terrain = new Terrain(sTerrain, L"Terrain/Gray256.png");
		terrain->BaseMap(L"Terrain/Dirt3.png");
		terrain->LayerMap(L"Terrain/Grass (Hill).jpg", L"Terrain/Gray256.png");
		terrain->LayerMap2(L"Terrain/Path (Rocky).jpg", L"Terrain/Grass (Rocky).jpg");
		terrain->LayerMap3(L"Terrain/Cliff (Sandstone).jpg", L"Terrain/Sand (with pebbles).jpg");
		//terrain->Pass(2);
	}

	//TerrainLod
	{
		sTerrainLod = new Shader(L"49_TerrainLod.fxo");

		TerrainLod::InitializeDesc desc =
		{
			sTerrainLod,
			L"Terrain/Gray512.png",
			1.0f,
			16,
			5
		};

		terrainLod = new TerrainLod(desc);
		terrainLod->BaseMap(L"Terrain/Dirt.png");
		terrainLod->LayerMap(L"Terrain/Grass (Lawn).jpg", L"Terrain/Gray512.png");
		terrainLod->NormalMap(L"Terrain/Dirt_Normal.png");
		terrainLod->Pass(0);
	}

	//Billboard
	{
		bbShader = new Shader(L"25_Billboard.fxo");
		bb = new Billboard(bbShader);
		bb2 = new Billboard(bbShader);
		bb3 = new Billboard(bbShader);
		bb4 = new Billboard(bbShader);
		bb5 = new Billboard(bbShader);
		bb6 = new Billboard(bbShader);
		bb7 = new Billboard(bbShader);
		bb8 = new Billboard(bbShader);

		bb->AddTexture(L"Terrain/BillboardTexture1.png");
		bb2->AddTexture(L"Terrain/BillboardTexture2.png");
		bb3->AddTexture(L"Terrain/BillboardTexture3.png");
		bb4->AddTexture(L"Terrain/Tree.png");
		bb5->AddTexture(L"Terrain/Tree2.png");
		bb6->AddTexture(L"Terrain/Tree3.png");
		bb7->AddTexture(L"Terrain/Tree4.png");
		bb8->AddTexture(L"Terrain/Tree5.png");
	}

	Mesh();
	Airplane();
	Kachujin();

	//AddPointLights();
	//AddSpotLights();
}

void WaterDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(shadow);
	SafeDelete(terrain);
	SafeDelete(terrainLod);
	SafeDelete(sTerrain);
}

void WaterDemo::Update()
{
	

	ImGui::Begin("Terrain", nullptr);
	{
		ImGui::SliderInt("Terrain", (int *)&terrain_num, 0, 2);
		ImGui::Separator();
		if (terrain_num == 1)
		{
			ImGui::Checkbox("Billboard", &is_billboard);
			ImGui::Separator();
			terrain->Update();
			if (!is_billboard) {}
			else if (is_billboard)
				BillboardLayer();

		}
		else if (terrain_num == 2)
		{
			ImGui::Checkbox("WireFrame", &is_wireframe);
			terrainLod->Update();
			if (!is_wireframe)
				terrainLod->Pass(0);
			else if (is_wireframe)
				terrainLod->Pass(1);
		}
	}
	ImGui::End();

	ImGui::Begin("Mesh", nullptr);
	{
		ImGui::Checkbox("Mesh", &is_mesh);
		if (is_mesh)
		{
			sphere->Update();
			cylinder->Update();
			cube->Update();
			grid->Update();
		}
		else if (!is_mesh)
		{

		}
	}
	ImGui::End();
	
	ImGui::Begin("Model", nullptr);
	{
		ImGui::Checkbox("Model", &is_model);
		if (is_model)
		{
			airplane->Update();
			kachujin->Update();
		}
		else if (!is_model)
		{

		}
	}
	ImGui::End();
	
	ImGui::Begin("Sky", nullptr);
	{
		sky->Update();
	}
	ImGui::End();
		
	ImGui::Begin("Water", nullptr);
	{
		ImGui::Checkbox("Water", &is_water);
		water->Update();
	}
	ImGui::End();

	ImGui::Begin("Ocean", nullptr);
	{
		ImGui::Checkbox("Ocean", &is_ocean);
		ocean->Update(Time::Get()->Running());
	}
	ImGui::End();

	ImGui::Begin("Weather", nullptr);
	{
		ImGui::Checkbox("Weather", &is_weather);
		if (is_weather)
		{
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
		else if (!is_weather)
		{

		}
	}
	ImGui::End();
	
	ImGui::Begin("Light", nullptr);
	{
		if (ImGui::Button("AddPointLight"))
			AddPointLights();

		if (ImGui::Button("AddSpotLight"))
			AddSpotLights();

		ImGui::Checkbox("Light", &is_light);
		if (is_light)
		{
			
		}
		else if (!is_light)
		{

		}
	}
	ImGui::End();
	
	bb->Update();
	bb2->Update();
	bb3->Update();
	bb4->Update();
	bb5->Update();
	bb6->Update();
	bb7->Update();
	bb8->Update();
}

void WaterDemo::PreRender()
{
	sky->PreRender();

	//Depth
	{
		shadow->Set();

		Pass(0, 1, 2);

		if (is_mesh)
		{
			wall->Render();
			sphere->Render();
			brick->Render();
			cylinder->Render();

			stone->Render();
			cube->Render();

			floor->Render();
			grid->Render();
		}
		else if (!is_mesh)
		{

		}

		if (is_model)
		{
			airplane->Render();
			kachujin->Render();
		}
		else if (!is_model)
		{

		}
		bb->Render();
		bb2->Render();
		bb3->Render();
		bb4->Render();
		bb5->Render();
		bb6->Render();
		bb7->Render();
		bb8->Render();
	}

	
	//Reflection
	{
		water->PreRender_Reflection();

		sky->Pass(10, 11, 12);
		sky->Render();

		Pass(13, 14, 15);
		if (is_mesh)
		{
			wall->Render();
			sphere->Render();
			brick->Render();
			cylinder->Render();

			stone->Render();
			cube->Render();

			floor->Render();
			grid->Render();
		}
		else if (!is_mesh)
		{

		}

		if (is_model)
		{
			airplane->Render();
			kachujin->Render();
		}
		else if (!is_model)
		{

		}
		bb->Render();
		bb2->Render();
		bb3->Render();
		bb4->Render();
		bb5->Render();
		bb6->Render();
		bb7->Render();
		bb8->Render();
	}

	//Refraction
	{
		water->PreRender_Refraction();

		sky->Pass(4, 5, 6);
		sky->Render();

		Pass(7, 8, 9);
		
		if (is_mesh)
		{
			wall->Render();
			sphere->Render();
			brick->Render();
			cylinder->Render();

			stone->Render();
			cube->Render();

			floor->Render();
			grid->Render();
		}
		else if (!is_mesh)
		{

		}

		if (is_model)
		{
			airplane->Render();
			kachujin->Render();
		}
		else if (!is_model)
		{

		}
		bb->Render();
		bb2->Render();
		bb3->Render();
		bb4->Render();
		bb5->Render();
		bb6->Render();
		bb7->Render();
		bb8->Render();
	}
}

void WaterDemo::Render()
{
	if (is_ocean)
	{
		ocean->Render(Time::Get()->Running());
	}

	sky->Pass(4, 5, 6);
	sky->Render();

	if (terrain_num == 1)
		terrain->Render();
	else if (terrain_num == 2)
		terrainLod->Render();

	

	if (is_water)
	{
		water->Pass(16);
		water->Render();
	}

	

	Pass(7, 8, 9);
	if (is_mesh)
	{
		wall->Render();
		sphere->Render();
		brick->Render();
		cylinder->Render();

		stone->Render();
		cube->Render();

		floor->Render();
		grid->Render();
	}
	else if (!is_mesh)
	{

	}

	if (is_model)
	{
		airplane->Render();
		kachujin->Render();
	}
	else if (!is_model)
	{

	}

	if (is_weather)
	{
		switch (weather)
		{
		case Weather::Rain: rain->Render(); break;
		case Weather::Snow: snow->Render(); break;
		}
	}
	else if (!is_weather)
	{

	}	

	//Height Text
	Vector3 picked = terrainLod->GetPickedPosition();
	string str = to_string(picked.x) + ", " + to_string(picked.y) + ", " + to_string(picked.z);
	Gui::Get()->RenderText(Vector2(10, 60), Color(1, 0, 0, 1), "Picked : " + str);

	
	bb->Render();
	bb2->Render();
	bb3->Render();
	bb4->Render();
	bb5->Render();
	bb6->Render();
	bb7->Render();
	bb8->Render();	
}

void WaterDemo::PostRender()
{
	//sky->PostRender();
	
	if(is_ocean)
		ocean->RenderFFT();
}

void WaterDemo::Mesh()
{
	//Create Material
	{
		floor = new Material(shader);
		floor->DiffuseMap("Floor.png");
		floor->SpecularMap("Floor_Specular.png");
		floor->NormalMap("Floor_Normal.png");
		floor->Specular(1, 1, 1, 20);
		floor->Emissive(0.15f, 0.15f, 0.15f, 0.3f);

		stone = new Material(shader);
		stone->DiffuseMap("Stones.png");
		stone->SpecularMap("Stones_Specular.png");
		stone->NormalMap("Stones_Normal.png");
		stone->Specular(1, 1, 1, 20);
		stone->Emissive(0.15f, 0.15f, 0.15f, 0.3f);


		brick = new Material(shader);
		brick->DiffuseMap("Bricks.png");
		brick->SpecularMap("Bricks_Specular.png");
		brick->NormalMap("Bricks_Normal.png");
		brick->Specular(1, 0.3f, 0.3f, 20);
		brick->Emissive(0.15f, 0.15f, 0.15f, 0.3f);

		wall = new Material(shader);
		wall->DiffuseMap("Wall.png");
		wall->SpecularMap("Wall_Specular.png");
		wall->NormalMap("Wall_Normal.png");
		wall->Specular(1, 1, 1, 20);
		wall->Emissive(0.15f, 0.15f, 0.15f, 0.3f);
	}

	//Create Mesh
	{
		Transform* transform = NULL;

		cube = new MeshRender(shader, new MeshCube());
		transform = cube->AddTransform();
		transform->Position(128.0f + 0, 5, 128.0f + 0);
		transform->Scale(20, 10, 20);

		grid = new MeshRender(shader, new MeshGrid(15, 15));
		transform = grid->AddTransform();
		transform->Position(128.0f + 0, 0, 128.0f + 0);
		transform->Scale(20, 1, 20);

		cylinder = new MeshRender(shader, new MeshCylinder(0.5f, 3.0f, 20, 20));
		sphere = new MeshRender(shader, new MeshSphere(0.5f, 20, 20));
		for (UINT i = 0; i < 5; i++)
		{
			transform = cylinder->AddTransform();
			transform->Position(128.0f + -30, 6, 128.0f + -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);

			transform = cylinder->AddTransform();
			transform->Position(128.0f + 30, 6, 128.0f + -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);


			transform = sphere->AddTransform();
			transform->Position(128.0f + -30, 15.5f, 128.0f + -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);

			transform = sphere->AddTransform();
			transform->Position(128.0f + 30, 15.5f, 128.0f + -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);
		}
	}

	sphere->UpdateTransforms();
	cylinder->UpdateTransforms();
	cube->UpdateTransforms();
	grid->UpdateTransforms();

	meshes.push_back(sphere);
	meshes.push_back(cylinder);
	meshes.push_back(cube);
	meshes.push_back(grid);
}

void WaterDemo::Airplane()
{
	airplane = new ModelRender(shader);
	airplane->ReadMaterial(L"B787/Airplane");
	airplane->ReadMesh(L"B787/Airplane");

	Transform* transform = airplane->AddTransform();
	transform->Position(128.0f + 2.0f, 9.91f, 128.0f + 2.0f);
	transform->Scale(0.004f, 0.004f, 0.004f);
	airplane->UpdateTransforms();

	models.push_back(airplane);
}

void WaterDemo::Kachujin()
{
	weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Sword");
	weapon->ReadMesh(L"Weapon/Sword");

	kachujin = new ModelAnimator(shader);
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Running");
	kachujin->ReadClip(L"Kachujin/Jump");
	kachujin->ReadClip(L"Kachujin/Hip_Hop_Dancing");

	Transform attachTransform;

	attachTransform.Position(-10, 0, -10);
	attachTransform.Scale(1.0f, 1.0f, 1.0f);

	kachujin->GetModel()->Attach(shader, weapon, 35, &attachTransform);


	Transform* transform = NULL;

	transform = kachujin->AddTransform();
	transform->Position(128.0f + -25, 0, 128.0f + -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayClip(0, 0, 0.25f);

	transform = kachujin->AddTransform();
	transform->Position(128.0f + -10, 0, 128.0f + -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayClip(1, 1, 1.0f);

	transform = kachujin->AddTransform();
	transform->Position(128.0f + 10, 0, 128.0f + -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayClip(2, 2, 0.75f);

	transform = kachujin->AddTransform();
	transform->Position(128.0f + 25, 0, 128.0f + -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayClip(3, 3, 0.35f);

	kachujin->UpdateTransforms();

	animators.push_back(kachujin);


	for (UINT i = 0; i < 4; i++)
	{
		colliders[i].Init = new Transform();
		colliders[i].Init->Scale(10, 10, 120);
		colliders[i].Init->Position(-10, 0, -60);
		
		colliders[i].Transform = new Transform();
		colliders[i].Collider = new Collider(colliders[i].Transform, colliders[i].Init);
	}
}

void WaterDemo::BillboardLayer()
{
	ImGui::SliderInt("Tree Layer", &treenum, 0, 7);

	if (Mouse::Get()->Press(0))
	{
		auto picked = terrain->GetPickedPosition();
		for (UINT i = 0; i < 10; i++)
		{
			Vector2 scale = Math::RandomVec2(5.0f, 10.0f);
			float positionx = Math::Random(picked.x - 10.0f, picked.x + 10.0f);
			float positionz = Math::Random(picked.z - 10.0f, picked.z + 10.0f);
			float positiony = terrain->GetHeight(Vector3(positionx, 0.0f, positionz)) + scale.y * 0.5f;
			float random = Math::Random(-1.0f, 1.0f);

			if (treenum == 0)
			{
				bb->Add(Vector3(positionx, positiony, positionz), scale, random);
			}
			else if (treenum == 1)
			{
				bb2->Add(Vector3(positionx, positiony, positionz), scale, random);
			}
			else if (treenum == 2)
			{
				bb3->Add(Vector3(positionx, positiony, positionz), scale, random);
			}
			else if (treenum == 3)
			{
				bb4->Add(Vector3(positionx, positiony, positionz), scale, random);
			}
			else if (treenum == 4)
			{
				bb5->Add(Vector3(positionx, positiony, positionz), scale, random);
			}
			else if (treenum == 5)
			{
				bb6->Add(Vector3(positionx, positiony, positionz), scale, random);
			}
			else if (treenum == 6)
			{
				bb7->Add(Vector3(positionx, positiony, positionz), scale, random);
			}
			else if (treenum == 7)
			{
				bb8->Add(Vector3(positionx, positiony, positionz), scale, random);
			}
		}
	}

}

void WaterDemo::Pass(UINT mesh, UINT model, UINT anim)
{
	for (MeshRender* temp : meshes)
		temp->Pass(mesh);

	for (ModelRender* temp : models)
		temp->Pass(model);

	for (ModelAnimator* temp : animators)
		temp->Pass(anim);
}

void WaterDemo::AddPointLights()
{
	PointLight light;

	for (float z = -30; z <= 30; z += 30)
	{
		for (float x = -30; x <= 30; x += 30)
		{
			light =
			{
				Color(0, 0, 0, 1),
				Math::RandomColor3(),
				Color(0, 0, 0, 1),
				Color(0, 0, 0, 1),
				Vector3(128.0f + x, 1, 128.0f + z), 5, Math::Random(0.1f, 1.0f)
			};

			Context::Get()->AddPointLight(light);
		}
	}


	light =
	{
		Color(0.0f, 0.0f, 0.0f, 1.0f), //Ambient
		Color(0.0f, 0.0f, 1.0f, 1.0f), //Diffuse
		Color(0.0f, 0.0f, 0.7f, 1.0f), //Specular
		Color(0.0f, 0.0f, 0.7f, 1.0f), //Emissive
		Vector3(128.0f + -25, 10, 128.0f + -30), 5.0f, 0.9f
	};
	Context::Get()->AddPointLight(light);

	light =
	{
		Color(0.0f, 0.0f, 0.0f, 1.0f),
		Color(1.0f, 0.0f, 0.0f, 1.0f),
		Color(0.6f, 0.2f, 0.0f, 1.0f),
		Color(0.6f, 0.3f, 0.0f, 1.0f),
		Vector3(128.0f + 25, 10,128.0f + -30), 10.0f, 0.3f
	};
	Context::Get()->AddPointLight(light);
}

void WaterDemo::AddSpotLights()
{
	SpotLight light;
	light =
	{
		Color(0.3f, 1.0f, 0.0f, 1.0f),
		Color(0.7f, 1.0f, 0.0f, 1.0f),
		Color(0.3f, 1.0f, 0.0f, 1.0f),
		Color(0.3f, 1.0f, 0.0f, 1.0f),
		Vector3(128.0f + -10, 20, 128.0f + -30), 25.0f,
		Vector3(0, -1, 0), 30.0f, 0.9f
	};
	Context::Get()->AddSpotLight(light);

	light =
	{
		Color(1.0f, 0.2f, 0.9f, 1.0f),
		Color(1.0f, 0.2f, 0.9f, 1.0f),
		Color(1.0f, 0.2f, 0.9f, 1.0f),
		Color(1.0f, 0.2f, 0.9f, 1.0f),
		Vector3(128.0f + 10, 20, 128.0f + -30), 30.0f,
		Vector3(0, -1, 0), 40.0f, 0.9f
	};
	Context::Get()->AddSpotLight(light);
}