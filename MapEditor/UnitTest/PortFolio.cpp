#include "stdafx.h"
#include "PortFolio.h"
#include <random>
#include "Utilities/BinaryFile.h"

void PortFolio::Initialize()
{
	//Context::Get()->GetCamera()->RotationDegree(29, 2, 0);
	//Context::Get()->GetCamera()->Position(124, 49, 10);
	Context::Get()->GetCamera()->RotationDegree(-4, 1114, 0);
	Context::Get()->GetCamera()->Position(126, 5, 125);
	((Freedom *)Context::Get()->GetCamera())->Speed(20, 2);


	shader = new Shader(L"48_Water.fxo");

	shadow = new Shadow(shader, Vector3(125, 0, 125), 50);
	

	//Sky
	{
		sky = new Sky(shader);
		sky->ScatteringPass(3);
		sky->RealTime(false, Math::PI - 1e-6f);
		//sky->RealTime(true, Math::PI - 1e-6f, 0.25f);
	}
	//Weather
	{
		rain = new Rain(Vector3(300, 300, 300), 10000, L"Environment/Rain.png");
		snow = new Snow(Vector3(300, 300, 300), 10000, L"Environment/Snow.png");
	}

	//Water
	{	
		water = new Water(shader, 127.5);
		water->GetTransform()->Position(128.0f + 0, 1.15f, 128.0f + 0);
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
		
		terrain = new Terrain(shader, L"Terrain/Gray256.png");
		terrain->BaseMap(L"Terrain/Dirt3.png");
		terrain->LayerMap(L"Terrain/Grass (Hill).jpg", L"Terrain/Gray256.png");
		terrain->LayerMap2(L"Terrain/Path (Rocky).jpg", L"Terrain/Grass (Rocky).jpg");
		terrain->LayerMap3(L"Terrain/Cliff (Sandstone).jpg", L"Terrain/Sand (with pebbles).jpg");
		terrain->Pass(19);
	}

	//TerrainLod
	{
		TerrainLod::InitializeDesc desc =
		{
			shader,
			L"Terrain/Gray512.png",
			1.0f,
			16,
			5
		};

		terrainLod = new TerrainLod(desc);
		terrainLod->BaseMap(L"Terrain/Dirt.png");
		terrainLod->LayerMap(L"Terrain/Grass (Lawn).jpg", L"Terrain/Gray512.png");
		terrainLod->NormalMap(L"Terrain/Dirt_Normal.png");
		terrainLod->Pass(20);
	}

	//Billboard
	{
		//bbShader = new Shader(L"25_Billboard.fxo");
		bb = new Billboard(shader);
		bb2 = new Billboard(shader);
		bb3 = new Billboard(shader);
		bb4 = new Billboard(shader);
		bb5 = new Billboard(shader);
		bb6 = new Billboard(shader);
		bb7 = new Billboard(shader);
		bb8 = new Billboard(shader);

		tex1 = new Texture(L"Terrain/BillboardTexture1.png");
		tex2 = new Texture(L"Terrain/BillboardTexture2.png");
		tex3 = new Texture(L"Terrain/BillboardTexture3.png");
		tex4 = new Texture(L"Terrain/Tree.png");
		tex5 = new Texture(L"Terrain/Tree2.png");
		tex6 = new Texture(L"Terrain/Tree3.png");
		tex7 = new Texture(L"Terrain/Tree4.png");
		tex8 = new Texture(L"Terrain/Tree5.png");
		
		bb->AddTexture(L"Terrain/BillboardTexture1.png");
		bb2->AddTexture(L"Terrain/BillboardTexture2.png");
		bb3->AddTexture(L"Terrain/BillboardTexture3.png");
		bb4->AddTexture(L"Terrain/Tree.png");
		bb5->AddTexture(L"Terrain/Tree2.png");
		bb6->AddTexture(L"Terrain/Tree3.png");
		bb7->AddTexture(L"Terrain/Tree4.png");
		bb8->AddTexture(L"Terrain/Tree5.png");

		bb->Pass(18);
		bb2->Pass(18);
		bb3->Pass(18);
		bb4->Pass(18);
		bb5->Pass(18);
		bb6->Pass(18);
		bb7->Pass(18);
		bb8->Pass(18);
	}

	//Particle
	{
		particle = new ParticleSystem(L"Star");
		particle1 = new ParticleSystem(L"Firecylinder");
		particle2 = new ParticleSystem(L"Firecylindersmoke");
		particle3 = new ParticleSystem(L"Firecylinderstar");
		particle4 = new ParticleSystem(L"Spark2");
		particle5 = new ParticleSystem(L"Trail_2");		

		meteor_particle1 = new ParticleSystem(L"Meteor_1");
		meteor_particle2 = new ParticleSystem(L"Meteor_2");
		meteor_particle3 = new ParticleSystem(L"Meteor_3");

	}
	Mesh();
	Weapon();
	Dreyar();
	CastleGuardSword();
	CastleGuardBow();

	std::mt19937 engine((unsigned int)time(NULL));               // MT19937 난수 엔진
	std::uniform_int_distribution<int> distribution(1, 4);       // 생성 범위
	auto generator = bind(distribution, engine);

	std::mt19937 engine2((unsigned int)time(NULL));               // MT19937 난수 엔진
	std::uniform_real_distribution<float> distribution2(0.01f, 0.03f);       // 생성 범위
	auto generator2 = bind(distribution2, engine2);

	for (int i = 0; i < 10; i++)
	{
		clip[i] = 2;

		is_mon_heat[i] = false;
		is_mon_attack[i] = false;
		is_mon_patrol[i] = true;
		is_mon_death[i] = false;
		is_mon_running[i] = false;
		is_mon_running_to_waypoint[i] = false;

		is_collision_sword_mon[i] = false;
		is_collision_arrow_mon[i] = false;

		mon_hp[i] = 100.0f;

		if (i < 5)
		{
			castleGuardSword->GetTransform(i)->Position(&mon_waypoint[i]);
			castleGuardSword->GetTransform(i)->RotationDegree(mon_rotation[i]);
			is_monarrow_initialize[i] = false;
			is_monarrow_moving[i] = false;
			is_collision_sword[i] = false;
			monarrow_position[i] = Vector3(0, 0, 0);
			monarrow_rotation[i] = Vector3(0, 0, 0);
		}
		else if (i >= 5)
		{
			castleGuardBow->GetTransform(i - 5)->Position(&mon_waypoint[i]);
			castleGuardBow->GetTransform(i - 5)->RotationDegree(mon_rotation[i]);
		}
		mon_direction[i] = generator();
		switch (mon_direction[i])
		{
		case 1: mon_rotation[i] = Vector3(0, -90.0f, 0); break;
		case 2: mon_rotation[i] = Vector3(0, 90.0f, 0); break;
		case 3: mon_rotation[i] = Vector3(0, 180.0f, 0); break;
		case 4: mon_rotation[i] = Vector3(0, 0.0f, 0); break;
		}
		mon_speed[i] = generator2();
	}
	for (int i = 0; i < 40; i++)
	{
		is_collision_arrow[i] = false;
	}

	is_running = false;
	is_attacking = false;
	speed = 10.0f;

	

	//AddPointLights();
	//AddSpotLights();
}

void PortFolio::Destroy()
{
	SafeDelete(shader);
	SafeDelete(shadow);
	SafeDelete(terrain);
	SafeDelete(terrainLod);
	SafeDelete(sOcean);
	SafeDelete(bb);
	SafeDelete(bb2);
	SafeDelete(bb3);
	SafeDelete(bb4);
	SafeDelete(bb5);
	SafeDelete(bb6);
	SafeDelete(bb7);
	SafeDelete(bb8);
	SafeDelete(sky);
	SafeDelete(snow);
	SafeDelete(rain);
	SafeDelete(water); 
	SafeDelete(particle);
	SafeDelete(particle1);
	SafeDelete(particle2);
	SafeDelete(particle3);
	SafeDelete(particle4);
	SafeDelete(particle5);
}

void PortFolio::Update()
{

	ImGui::SliderFloat3("Light Direction", Context::Get()->Direction(), -1, 1);

	ImGui::Begin("Terrain", nullptr, ImVec2(0, 250), 0.4f);
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
			{
				BillboardLayer();
			}
		}
		else if (terrain_num == 2)
		{
			ImGui::Checkbox("WireFrame", &is_wireframe);
			terrainLod->Update();
			if (!is_wireframe)
				terrainLod->Pass(20);
			else if (is_wireframe)
				terrainLod->Pass(21);
		}
	}
	ImGui::End();
	

	ImGui::Begin("Mesh", nullptr);
	{
		ImGui::Checkbox("Mesh", &is_mesh);
		if (is_mesh)
		{
			sphere->Update();
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
			//Vector3 campos;
			//dreyar->GetTransform(0)->Position(&campos);
			//Context::Get()->NewOrbitCamera(&campos);

			//kachujin->Update();
			castleGuardSword->Update();
			castleGuardBow->Update();

			if (dreyar != NULL)
			{
				Vector3 position;
				dreyar->GetTransform(0)->Position(&position);
				Vector3 rotation;
				dreyar->GetTransform(0)->Rotation(&rotation);

				((Freedom *)Context::Get()->GetCamera())->SetTarget(position);
				((Freedom *)Context::Get()->GetCamera())->SetOrbitCamera(true);

				//Orbit Camera 
				//Setting.
				/*ImGui::SliderFloat("cameraLatitude", &cameraLatitude, -10, 10);
				ImGui::SliderFloat("cameraLongitude", &cameraLongitude, -2.0f, -1.6f);
				ImGui::SliderFloat("orbitDistance", &orbitDistance, 10, 35);

				camerarotation.x = -sinf(cameraLatitude) * cosf(cameraLongitude);
				camerarotation.z = -sinf(cameraLongitude);
				camerarotation.y = cosf(cameraLatitude) * cosf(cameraLongitude);
				cameraposition.x = position.x - camerarotation.x * orbitDistance;
				cameraposition.y = position.y - camerarotation.y * orbitDistance;
				cameraposition.z = position.z - camerarotation.z * orbitDistance;

				Context::Get()->GetCamera()->Position(cameraposition);
				Context::Get()->GetCamera()->RotationDegree(camerarotation);*/

				if (particle != NULL)
				{
					particle->Add(position);
					particle->Update();
				}
				
				if (Keyboard::Get()->Press(VK_LBUTTON))
				{
					picked = terrain->GetPickedPosition();

					pickrotation = atan2f(picked.x - position.x, picked.z - position.z);

					is_running = true;
					is_particle_attack_two = false;
					is_particle_attack_one = false;
					is_attackcombo_one = false;
					is_attackcombo_two = false;
					is_attacking = false;
					is_heat = false;
					is_death = false;
				}

				//기본공격키
				if (Keyboard::Get()->Press(VK_SPACE))
				{
					is_attacking = true;
					//is_arrow_initialize = true;
					is_arrow_moving = false;

					is_particle_attack_two = false;
					is_particle_attack_one = false;
					is_attackcombo_one = false;
					is_attackcombo_two = false;
					is_running = false;
					is_heat = false;
					is_death = false;
				}

				//Player - Q key (소드어택콤보 1)
				if (Keyboard::Get()->Down(0x51))
				{
					is_attackcombo_one = true;

					is_particle_attack_two = false;
					is_particle_attack_one = false;
					is_attackcombo_two = false;
					is_attacking = false;
					is_running = false;
					is_heat = false;
					is_death = false;
				}

				//Player - W key (소드어택콤보 2)
				if (Keyboard::Get()->Down(0x57))
				{
					is_attackcombo_two = true;

					is_particle_attack_two = false;
					is_particle_attack_one = false;
					is_attackcombo_one = false;
					is_attacking = false;
					is_running = false;
					is_heat = false;
					is_death = false;
				}

				//Player - E key (파티클 공격 - 1)
				if (Keyboard::Get()->Down(0x45))
				{
					is_particle_attack_one = true;

					is_particle_attack_two = false;
					is_attackcombo_two = false;
					is_attackcombo_one = false;
					is_attacking = false;
					is_running = false;
					is_heat = false;
					is_death = false;
				}

				//Player - R key (파티클 공격 - 2)
				if (Keyboard::Get()->Press(0x52))
				{
					is_particle_attack_twice = true;
				}

				if (is_particle_attack_twice && Keyboard::Get()->Up(0x52))
				{
					is_particle_attack_twice = false;

					is_particle_attack_two = true;

					particleposition = terrain->GetPickedPosition();
					is_particle_attack_one = false;
					is_attackcombo_two = false;
					is_attackcombo_one = false;
					is_attacking = false;
					is_running = false;
					is_heat = false;
					is_death = false;
				}

				if (Keyboard::Get()->Down(VK_UP))
				{
					weapon_num++;
					weapon_num %= 3;
				}

				ImGui::SliderFloat("speed", &animspeed, 0, 2);
				ImGui::SliderFloat("taketime", &taketime, 0, 1);
				ImGui::SliderFloat("monattackrange", &monattackrange, 0, 1000);
				ImGui::SliderFloat("monbowattackrange", &monbowattackrange, 0, 1000);
				ImGui::SliderFloat("montracerange", &montracerange, 0, 1000);
				ImGui::SliderFloat("monwaybackrange", &monwaybackrange, 0, 1000);
				ImGui::SliderFloat("arrowposy", &arrowposy, 0, 10.0f);

				if (is_attackcombo_one) //q
				{
					if (weapon_num == 0)
					{
						if (playerClip != 5)
						{
							playerClip = 5;
							dreyar->PlayClip(0, playerClip, animspeed, taketime);
						}
					}
					else if (weapon_num == 1)
					{
						if (playerClip != 11)
						{
							playerClip = 11;
							dreyar->PlayClip(0, playerClip, animspeed, taketime);
						}
						int aaaaaa = dreyar->GetTime();
						ImGui::SliderInt("aa", &aaaaaa, 0, 200);

						if (dreyar->GetTime() == 13)
							is_trail = true;

						if (dreyar->GetTime() == 50)
							is_trail = false;

					}
				}
				else if (is_attackcombo_two) //w
				{
					if (weapon_num == 0)
					{
						if (playerClip != 6)
						{
							playerClip = 6;
							dreyar->PlayClip(0, playerClip, animspeed, taketime);
						}
					}
					else if (weapon_num == 1)
					{
						if (playerClip != 12)
						{
							playerClip = 12;
							dreyar->PlayClip(0, playerClip, animspeed, taketime);
						}

						int aaaaaa = dreyar->GetTime();
						ImGui::SliderInt("aa", &aaaaaa, 0, 200);


						if (dreyar->GetTime() == 13)
							is_trail = true;

						if (dreyar->GetTime() == 70)
							is_trail = false;
					}
				}
				else if (is_particle_attack_one && weapon_num == 0) //e
				{
					Vector3 meshposition;
					sphere->GetTransform(0)->Position(&meshposition);
					
					if (is_particle_attack_once)
					{
						meshposition = position;
						is_particle_attack_once = false;
					}

					{
						particle1->Add(meshposition);
						particle1->Update();
						particle2->Add(meshposition);
						particle2->Update();
						particle3->Add(meshposition);
						particle3->Update();
						particle4->Add(meshposition);
						particle4->Update();
					}

					if (playerClip != 13)
					{
						playerClip = 13;
						dreyar->PlayClip(0, playerClip, animspeed, taketime);
						sphere->GetTransform(0)->Position(dreyar->GetAttachTransform(0)._41, dreyar->GetAttachTransform(0)._42, dreyar->GetAttachTransform(0)._43);
						is_particle_attack_once = true;
					}
						
					meshposition.x += 0.15f * sinf(pickrotation);
					meshposition.z += 0.15f * cosf(pickrotation);

					sphere->GetTransform(0)->Position(meshposition);
					
				}
				else if (is_particle_attack_two && weapon_num == 0) //R
				{
					if (playerClip != 14)
					{
						playerClip = 14;
						dreyar->PlayClip(0, playerClip, animspeed, taketime);
						sphere->GetTransform(0)->Position(dreyar->GetAttachTransform(0)._41, dreyar->GetAttachTransform(0)._42, dreyar->GetAttachTransform(0)._43);
						is_particle_attack_once = true;
					}

					Vector3 meshposition;
					sphere->GetTransform(0)->Position(&meshposition);

					if (is_particle_attack_once)
					{
						meshposition = particleposition;
						particleposition.y = 10.0f;
						meshposition.y = 10.0f;
						is_particle_attack_once = false;
					}

					{
						meteor_particle1->Add(meshposition);
						meteor_particle1->Update();
						meteor_particle2->Add(meshposition);
						meteor_particle2->Update();
						meteor_particle3->Add(meshposition);
						meteor_particle3->Update();
					}

					meshposition.y -= 0.15f;
					particleposition.y -= 0.15f;

					sphere->GetTransform(0)->Position(meshposition);

					
				}
				else if (is_attacking)
				{
					if (weapon_num == 0)
					{
						if (playerClip != 4)
						{
							playerClip = 4;
							dreyar->PlayClip(0, playerClip, animspeed, taketime);
						}
					}
					else if (weapon_num == 1)
					{
						if (playerClip  != 10)
						{
							playerClip  = 10;	
							dreyar->PlayClip(0, playerClip, animspeed, taketime);
						}
						
						if (dreyar->GetTime() == 13)
							is_trail = true;
						
						if (dreyar->GetTime() == 30)
							is_trail = false;
					}
					else if (weapon_num == 2)
					{
						if (playerClip  != 17)
						{
							playerClip  = 17;
							dreyar->PlayClip(0, playerClip, animspeed * 2, taketime);
						}						

						if (!is_arrow_initialize)
						{
							if (playerClip != 17)
							{
								playerClip = 17;
								dreyar->PlayClip(0, playerClip, animspeed, taketime);
							}

							arrow_position = position;
							arrow_position.y = position.y + arrowposy;

							arrow_rotation.x = rotation.x;
							arrow_rotation.y = rotation.y;
							arrow_rotation.z = rotation.z;
							weaponArrow->GetTransform(0)->Rotation(arrow_rotation.x - 1.56f, arrow_rotation.y, arrow_rotation.z - 3.14f);
							is_arrow_initialize = true;
							is_arrow_moving = true;
						}
					}
				}//Attacking
				else if (is_heat)
				{
					if (weapon_num == 0)
					{
						if (playerClip != 7)
						{
							playerClip = 7;
							dreyar->PlayClip(0, playerClip, animspeed, taketime);
						}
					}
					else if (weapon_num == 1)
					{
						if (playerClip != 7)
						{
							playerClip = 7;
							dreyar->PlayClip(0, playerClip, animspeed, taketime);

						}
					}
					else if (weapon_num == 2)
					{
						if (playerClip != 7)
						{
							playerClip = 7;
							dreyar->PlayClip(0, playerClip, animspeed, taketime);
						}
					}
				}//Is_heat
				else if (is_running)
				{
					if (playerClip  != 1)
					{
						playerClip  = 1;
						dreyar->PlayClip(0, playerClip, animspeed, taketime);
					}

					if (abs(pickrotation - rotation.y) > 0.1f)
					{
						rotation.y = pickrotation - 3.14f;
					}

					position.x += 0.15f * sinf(pickrotation);
					position.y = terrain->GetHeight(position);
					position.z += 0.15f * cosf(pickrotation);

					if (abs(picked.x - position.x) < 0.2f && abs(picked.z - position.z) < 0.2f)
					{
						is_running = false;
						is_attacking = false;

						if (playerClip  != 0)
						{
							playerClip  = 0;
							dreyar->PlayClip(0, playerClip, animspeed, taketime);
						}
					}
				}//Running
				else  
				{
					if (weapon_num == 0)
					{
						if (playerClip  != 3)
						{
							playerClip  = 3;
							dreyar->PlayClip(0, playerClip, animspeed, taketime);
						}
					}
					else 
					{
						if (playerClip  != 0)
						{
							playerClip  = 0;
							dreyar->PlayClip(0, playerClip, animspeed, taketime);
						}
					}
				}
				//Idle


				if (is_arrow_moving && is_arrow_initialize)
				{
					arrow_position.x -= 1.5f * sinf(arrow_rotation.y);
					arrow_position.z -= 1.5f * cosf(arrow_rotation.y);

					weaponArrow->GetTransform(0)->Position(arrow_position);
					//weaponArrow->UpdateTransforms();
				}
				
				if (is_attacking && dreyar->GetTime() == 40)
				{
					is_arrow_initialize = false;
					is_arrow_moving = true;
				}

				


				dreyar->GetTransform(0)->Position(position);
				dreyar->GetTransform(0)->Rotation(rotation);
				
				//castleGuardSword
				for (int i = 0; i < 5; i++)
				{
					if (is_mon_heat[i])
					{
						is_mon_attack[i] = false;
						is_mon_patrol[i] = false;
					}
					/*else if (is_attackcombo_two && powf(mon_position[i].x - position.x, 2) + powf(mon_position[i].z - position.z, 2) < sword_jump_attack_range &&
						dreyar->GetTime() > 45)
					{
						is_mon_heat[i] = true;
					}*/
					else if (powf(mon_position[i].x - position.x, 2) + powf(mon_position[i].z - position.z, 2) < montracerange &&
						powf(mon_position[i].x - position.x, 2) + powf(mon_position[i].z - position.z, 2) > monattackrange)
					{
						is_mon_running[i] = true;
					}
					else if (powf(mon_position[i].x - position.x, 2) + powf(mon_position[i].z - position.z, 2) <= monattackrange)
					{
						is_mon_running[i] = false;
						is_mon_attack[i] = true;
						is_mon_patrol[i] = false;
					}
					else
					{
						is_mon_attack[i] = false;
						is_mon_patrol[i] = true;
					}

					if (is_mon_heat[i])
					{
						is_mon_patrol[i] = false;
						is_mon_attack[i] = false;
						mon_hp[i] = 0.0f;

						if (clip[i] != 5)
						{
							clip[i] = 5;
							castleGuardSword->PlayClip(i, clip[i], monanimspeed, montaketime);
						}

						is_mon_death[i] = true;
					}
					else if (is_mon_attack[i] && mon_hp[i] > 0.0f)
					{
						if (clip[i] != 2)
						{
							clip[i] = 2;
							castleGuardSword->PlayClip(i, clip[i], monanimspeed, montaketime);
						}
						
						if (powf(mon_position[i].x - position.x, 2) + powf(mon_position[i].z - position.z, 2) > monattackrange)
						{
							is_mon_running[i] = true;
							is_mon_attack[i] = false;
						}
					}
					else if (is_mon_running[i] && mon_hp[i] > 0.0f && !is_mon_running_to_waypoint[i])
					{
						if (clip[i] != 1)
						{
							clip[i] = 1;							
							castleGuardSword->PlayClip(i, clip[i], monanimspeed, montaketime);
						}
						float rotationy = atan2f(mon_position[i].x - position.x, mon_position[i].z - position.z);
						castleGuardSword->GetTransform(i)->Rotation(mon_rotation[i].x, rotationy, mon_rotation[i].z);
						mon_position[i].x -= 0.15f * sinf(rotationy);
						mon_position[i].z -= 0.15f * cosf(rotationy);
						castleGuardSword->GetTransform(i)->Position(mon_position[i]);

						if (powf(mon_position[i].x - mon_waypoint[i].x, 2) + powf(mon_position[i].z - mon_waypoint[i].z, 2) > monwaybackrange)
						{
							is_mon_running_to_waypoint[i] = true;
							is_mon_running[i] = false;
							is_mon_attack[i] = false;
						}

						if (powf(mon_position[i].x - position.x, 2) + powf(mon_position[i].z - position.z, 2) <= monattackrange)
						{
							is_mon_running[i] = false;
							is_mon_attack[i] = true;
						}
					}
					else if (is_mon_running_to_waypoint[i] && mon_hp[i] > 0.0f)
					{
						if (clip[i] != 1)
						{
							clip[i] = 1;
							castleGuardSword->PlayClip(i, clip[i], monanimspeed, montaketime);
						}
						float rotationy = atan2f(mon_position[i].x - mon_waypoint[i].x, mon_position[i].z - mon_waypoint[i].z);
						castleGuardSword->GetTransform(i)->Rotation(mon_rotation[i].x, rotationy, mon_rotation[i].z);
						mon_position[i].x -= 0.15f * sinf(rotationy);
						mon_position[i].z -= 0.15f * cosf(rotationy);
						castleGuardSword->GetTransform(i)->Position(mon_position[i]);

						if (powf(mon_position[i].x - mon_waypoint[i].x, 2) + powf(mon_position[i].z - mon_waypoint[i].z, 2) < 10.0f)
						{
							is_mon_running_to_waypoint[i] = false;
							is_mon_patrol[i] = true;
						}
					}
					else if (is_mon_patrol[i] && mon_hp[i] > 0.0f)
					{
						if (clip[i] != 1)
						{
							clip[i] = 1;
							castleGuardSword->PlayClip(i, clip[i], monanimspeed, montaketime);
						}

						castleGuardSword->GetTransform(i)->Position(&mon_position[i]);
						castleGuardSword->GetTransform(i)->RotationDegree(&mon_rotation[i]);

						switch (mon_direction[i])
						{
						case 1: mon_position[i].x += mon_speed[i]; if (mon_rotation[i].y - 90.0f > -180.0f) { mon_rotation[i].y -= 10.0f; } break;
						case 2:	mon_position[i].x -= mon_speed[i]; if (mon_rotation[i].y + 90.0f < 180.0f) { mon_rotation[i].y += 10.0f; } break;
						case 3: mon_position[i].z += mon_speed[i]; if (mon_rotation[i].y + 180.0f < 360.0f) { mon_rotation[i].y += 10.0f; } break;
						case 4: mon_position[i].z -= mon_speed[i]; if (mon_rotation[i].y - 0.0f > 0.0f) { mon_rotation[i].y -= 10.0f; } break;
						}

						if (mon_position[i].x - mon_waypoint[i].x > 10.0f)
						{
							mon_direction[i] = 2;
						}
						else if (mon_position[i].x - mon_waypoint[i].x < -10.0f)
						{
							mon_direction[i] = 1;
						}

						if (mon_position[i].z - mon_waypoint[i].z > 10.0f)
						{
							mon_direction[i] = 4;
						}
						else if (mon_position[i].z - mon_waypoint[i].z < -10.0f)
						{
							mon_direction[i] = 3;
						}

						//castleGuardSword->GetTransform(i)->Position(mon_position[i]);
						castleGuardSword->GetTransform(i)->RotationDegree(mon_rotation[i]);
					}

					if (is_mon_death[i])
					{
						if (castleGuardSword->GetTime(i) == 55)
							castleGuardSword->SetStopAnim(i, true);
					}
					mon_position[i].y = terrain->GetHeight(mon_position[i]);
					castleGuardSword->GetTransform(i)->Position(mon_position[i]);

				}

				//castleGuardBow
				for (int i = 5; i < 10; i++)
				{
					if (is_mon_heat[i])
					{
						is_mon_attack[i] = false;
						is_mon_patrol[i] = false;
					}
					/*else if (is_attackcombo_two && powf(mon_position[i].x - position.x, 2) + powf(mon_position[i].z - position.z, 2) < sword_jump_attack_range &&
						dreyar->GetTime() > 45)
					{
						is_mon_heat[i] = true;
					}*/
					else if (powf(mon_position[i].x - position.x, 2) + powf(mon_position[i].z - position.z, 2) <= monbowattackrange)
					{
						is_mon_running[i] = false;
						is_mon_attack[i] = true;
						is_mon_patrol[i] = false;
					}
					else if (powf(mon_position[i].x - position.x, 2) + powf(mon_position[i].z - position.z, 2) < montracerange &&
						powf(mon_position[i].x - position.x, 2) + powf(mon_position[i].z - position.z, 2) > monbowattackrange)
					{
						is_mon_running[i] = true;
						is_mon_attack[i] = false;
						is_mon_patrol[i] = false;
					}
					else
					{
						is_mon_running[i] = false;
						is_mon_attack[i] = false;
						is_mon_patrol[i] = true;
					}

					if (is_mon_heat[i])
					{
						is_mon_patrol[i] = false;
						is_mon_attack[i] = false;

						mon_hp[i] = 0.0f;

						if (clip[i] != 5)
						{
							clip[i] = 5;
							castleGuardBow->PlayClip(i - 5, clip[i], monanimspeed, montaketime);
						}

						is_mon_death[i] = true;
					}
					else if (is_mon_running[i] && mon_hp[i] > 0.0f && !is_mon_running_to_waypoint[i])
					{
						if (clip[i] != 1)
						{
							clip[i] = 1;
							castleGuardBow->PlayClip(i - 5, clip[i], monanimspeed, montaketime);
						}
						float rotationy = atan2f(mon_position[i].x - position.x, mon_position[i].z - position.z);
						castleGuardBow->GetTransform(i - 5)->Rotation(mon_rotation[i].x, rotationy, mon_rotation[i].z);
						mon_position[i].x -= 0.15f * sinf(rotationy);
						mon_position[i].z -= 0.15f * cosf(rotationy);
						castleGuardBow->GetTransform(i - 5)->Position(mon_position[i]);

						if (powf(mon_position[i].x - mon_waypoint[i].x, 2) + powf(mon_position[i].z - mon_waypoint[i].z, 2) > monwaybackrange)
						{
							is_mon_running_to_waypoint[i] = true;
							is_mon_running[i] = false;
							is_mon_attack[i] = false;
						}
						if (powf(mon_position[i].x - position.x, 2) + powf(mon_position[i].z - position.z, 2) <= monbowattackrange)
						{
							is_mon_running[i] = false;
							is_mon_attack[i] = true;
						}
					}
					else if (is_mon_running_to_waypoint[i] && mon_hp[i] > 0.0f)
					{
						if (clip[i] != 1)
						{
							clip[i] = 1;
							castleGuardBow->PlayClip(i - 5, clip[i], monanimspeed, montaketime);
						}
						float rotationy = atan2f(mon_position[i].x - mon_waypoint[i].x, mon_position[i].z - mon_waypoint[i].z);
						castleGuardBow->GetTransform(i - 5)->Rotation(mon_rotation[i].x, rotationy, mon_rotation[i].z);
						mon_position[i].x -= 0.15f * sinf(rotationy);
						mon_position[i].z -= 0.15f * cosf(rotationy);
						castleGuardBow->GetTransform(i - 5)->Position(mon_position[i]);

						if (powf(mon_position[i].x - mon_waypoint[i].x, 2) + powf(mon_position[i].z - mon_waypoint[i].z, 2) < 10.0f)
						{
							is_mon_running_to_waypoint[i] = false;
							is_mon_patrol[i] = true;
						}
					}
					else if (is_mon_attack[i] && mon_hp[i] > 0.0f)
					{
						float rotationarrow = atan2f(mon_position[i].x - position.x, mon_position[i].z - position.z);
						mon_rotation[i].y = rotationarrow;
						castleGuardBow->GetTransform(i - 5)->Rotation(mon_rotation[i].x, rotationarrow, mon_rotation[i].z);

						if (!is_monarrow_initialize[i - 5])
						{
							monarrow_position[i - 5] = mon_position[i];
							monarrow_position[i - 5].y = mon_position[i].y + monarrowposy;

							monarrow_rotation[i - 5].x = mon_rotation[i].x;
							monarrow_rotation[i - 5].y = rotationarrow;
							monarrow_rotation[i - 5].z = mon_rotation[i].z;
							weaponArrow->GetTransform(i - 4)->Rotation(monarrow_rotation[i - 5].x - 1.56f, monarrow_rotation[i - 5].y, monarrow_rotation[i - 5].z - 3.14f);
							is_monarrow_initialize[i - 5] = true;
							is_monarrow_moving[i - 5] = true;
						}

						if (clip[i] != 4)
						{
							clip[i] = 4;
							castleGuardBow->PlayClip(i - 5, clip[i], monanimspeed * 2, montaketime);
						}
					}
					else if (is_mon_patrol[i] && mon_hp[i] > 0.0f)
					{
						if (clip[i] != 1)
						{
							clip[i] = 1;
							castleGuardBow->PlayClip(i - 5, clip[i], monanimspeed, montaketime);
						}

						castleGuardBow->GetTransform(i - 5)->Position(&mon_position[i]);
						castleGuardBow->GetTransform(i - 5)->RotationDegree(&mon_rotation[i]);

						switch (mon_direction[i])
						{
						case 1: mon_position[i].x += mon_speed[i]; if (mon_rotation[i].y - 90.0f > -180.0f) { mon_rotation[i].y -= 10.0f; } break;
						case 2:	mon_position[i].x -= mon_speed[i]; if (mon_rotation[i].y + 90.0f < 180.0f) { mon_rotation[i].y += 10.0f; } break;
						case 3: mon_position[i].z += mon_speed[i]; if (mon_rotation[i].y + 180.0f < 360.0f) { mon_rotation[i].y += 10.0f; } break;
						case 4: mon_position[i].z -= mon_speed[i]; if (mon_rotation[i].y - 0.0f > 0.0f) { mon_rotation[i].y -= 10.0f; } break;
						}

						if (mon_position[i].x - mon_waypoint[i].x > 10.0f)
						{
							mon_direction[i] = 2;
						}
						else if (mon_position[i].x - mon_waypoint[i].x < -10.0f)
						{
							mon_direction[i] = 1;
						}

						if (mon_position[i].z - mon_waypoint[i].z > 10.0f)
						{
							mon_direction[i] = 4;
						}
						else if (mon_position[i].z - mon_waypoint[i].z < -10.0f)
						{
							mon_direction[i] = 3;
						}

						//castleGuardBow->GetTransform(i - 5)->Position(mon_position[i]);
						castleGuardBow->GetTransform(i - 5)->RotationDegree(mon_rotation[i]);
					}

					if (is_monarrow_moving[i - 5] && is_monarrow_initialize[i - 5])
					{
						monarrow_position[i - 5].x -= 1.5f * sinf(monarrow_rotation[i - 5].y);
						monarrow_position[i - 5].z -= 1.5f * cosf(monarrow_rotation[i - 5].y);

						weaponArrow->GetTransform(i - 4)->Position(monarrow_position[i - 5]);
					}

					if (is_mon_attack[i] && castleGuardBow->GetTime(i - 5) == 40)
					{
						is_monarrow_initialize[i - 5] = false;
						is_monarrow_moving[i - 5] = true;
					}

					if (is_mon_death[i])
					{
						if (castleGuardBow->GetTime(i - 5) == 55)
							castleGuardBow->SetStopAnim(i - 5, true);
					}
					mon_position[i].y = terrain->GetHeight(mon_position[i]);
					castleGuardBow->GetTransform(i - 5)->Position(mon_position[i]);

				}//bow

				sphere->UpdateTransforms();
				weaponArrow->UpdateTransforms();
				castleGuardBow->UpdateTransforms();
				castleGuardSword->UpdateTransforms();
				dreyar->UpdateTransforms();
				dreyar->Update();
				castleGuardSword->Update();
				castleGuardBow->Update();
				weaponArrow->Update();

				//Collision

				//Player
				{
					Matrix player_matrix = dreyar->GetTransform(0)->World();
					player_colliders.Collider->GetTransform()->World(player_matrix);
					player_colliders.Collider->Update();
				}

				//Player Sword
				{
					Matrix player_sword = dreyar->GetAttachTransform(0);
					player_sword_colliders.Collider->GetTransform()->World(player_sword);
					if (weapon_num == 1)
						player_sword_colliders.Collider->Update();

					Vector3 position2;
					Vector3 scale2;
					Vector3 rotation2;
					Math::MatrixDecompose(player_sword, scale2, rotation2, position2);

					if (particle5 != NULL && is_trail)
					{
						particle5->Add(position2);
						particle5->Update();
					}
				}

				//Monster Body
				for (int i = 0; i < 5; i++)
				{
					Matrix attach2 = castleGuardSword->GetTransform(i)->World();
					monster_colliders[i].Collider->GetTransform()->World(attach2);
					monster_colliders[i].Collider->Update();
				}
				for (int i = 5; i < 10; i++)
				{
					Matrix attach2 = castleGuardBow->GetTransform(i - 5)->World();
					monster_colliders[i].Collider->GetTransform()->World(attach2);
					monster_colliders[i].Collider->Update();
				}

				//Mon Sword
				for (int i = 0; i < 5; i++)
				{
					Matrix attach2 = castleGuardSword->GetAttachTransform(i);
					mon_sword_colliders[i].Collider->GetTransform()->World(attach2);
					mon_sword_colliders[i].Collider->Update();
				}

				//Arrow
				for (int i = 0; i < 6; i++)
				{
					Matrix attach3 = weaponArrow->GetTransform(i)->World();
					
					if(i == 0)
						attach3._42 = position.y + arrowposy + 0.1f;
					else
						attach3._42 = mon_position[i + 4].y + monarrowposy + 0.1f;

					arrow_colliders[i].Collider->GetTransform()->World(attach3);
					arrow_colliders[i].Collider->Update();
				}
			}
		}
		else if (!is_model)
		{
			((Freedom *)Context::Get()->GetCamera())->SetOrbitCamera(false);
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
		if (is_water)
		{
			Vector3 position;
			water->GetTransform()->Position(&position);
			ImGui::SliderFloat3("pos", position, 0, 5);
			water->GetTransform()->Position(position);
			water->Update();
		}
	}
	ImGui::End();

	ImGui::Begin("Ocean", nullptr);
	{
		ImGui::Checkbox("Ocean", &is_ocean);

		if(is_ocean)
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

void PortFolio::PreRender()
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
		}
		else if (!is_mesh)
		{

		}

		if (is_model)
		{
			//kachujin->Render();
			castleGuardSword->Render2();
			castleGuardBow->Render3();
			weaponArrow->Render();

			if (dreyar != NULL)
			{
				//dreyar->Pass(15);

				if (weapon_num == 0)
				{
					if (is_sword)
					{
						dreyar->GetModel()->Dettach(weaponSword, 36);

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

						dreyar->GetModel()->Attach(shader, weaponSword, 3, &attachspineTransform);

						is_unarmed = false;
						is_sword_spine = true;

					}
					dreyar->SetWeaponNum(1);

					dreyar->Render2();
				}
				else if (weapon_num == 2)
				{
					if (is_sword_spine)
					{
						dreyar->GetModel()->Dettach(weaponSword, 3);

						Transform attachTransform;
						attachTransform.Position(-60, -20, -200);
						attachTransform.Scale(10.0f, 10.0f, 10.0f);

						dreyar->GetModel()->Attach(shader, weaponSword, 36, &attachTransform);

						is_sword_spine = false;
						is_sword = true;

					}
					dreyar->SetWeaponNum(2);
					dreyar->Render3();
				}
			}
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

		if (terrain_num == 1)
		{
			//terrain->Pass(18);
			terrain->Render();
		}
		else if (terrain_num == 2)
		{
			terrainLod->Render();
		}
		else
		{

		}

		Pass(13, 14, 15);
		if (is_mesh)
		{
			wall->Render();
			sphere->Render();
		}
		else if (!is_mesh)
		{

		}

		if (is_model)
		{
			//kachujin->Render();
			castleGuardSword->Render2();
			castleGuardBow->Render3();
			weaponArrow->Render();

			if (dreyar != NULL)
			{
				//dreyar->Pass(15);

				if (weapon_num == 0)
				{
					if (is_sword)
					{
						dreyar->GetModel()->Dettach(weaponSword, 36);

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

						dreyar->GetModel()->Attach(shader, weaponSword, 3, &attachspineTransform);

						is_unarmed = false;
						is_sword_spine = true;

					}
					dreyar->SetWeaponNum(1);

					dreyar->Render2();
				}
				else if (weapon_num == 2)
				{
					if (is_sword_spine)
					{
						dreyar->GetModel()->Dettach(weaponSword, 3);

						Transform attachTransform;
						attachTransform.Position(-60, -20, -200);
						attachTransform.Scale(10.0f, 10.0f, 10.0f);

						dreyar->GetModel()->Attach(shader, weaponSword, 36, &attachTransform);

						is_sword_spine = false;
						is_sword = true;

					}
					dreyar->SetWeaponNum(2);
					dreyar->Render3();
				}
			}
		}
		else if (!is_model)
		{

		}
	}

	//Refraction
	{
		water->PreRender_Refraction();

		sky->Pass(4, 5, 6);
		sky->Render();

		if (terrain_num == 1)
		{
			//terrain->Pass(18);
			terrain->Render();
		}
		else if (terrain_num == 2)
		{
			terrainLod->Render();
		}
		else
		{

		}

		Pass(7, 8, 9);

		if (is_mesh)
		{
			wall->Render();
			sphere->Render();

		}
		else if (!is_mesh)
		{

		}

		if (is_model)
		{
			//kachujin->Render();
			weaponArrow->Render();
			castleGuardSword->Render2();
			castleGuardBow->Render3();
			if (dreyar != NULL)
			{
				//dreyar->Pass(15);

				if (weapon_num == 0)
				{
					if (is_sword)
					{
						dreyar->GetModel()->Dettach(weaponSword, 36);

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

						dreyar->GetModel()->Attach(shader, weaponSword, 3, &attachspineTransform);

						is_unarmed = false;
						is_sword_spine = true;
					}
					dreyar->SetWeaponNum(1);

					dreyar->Render2();
				}
				else if (weapon_num == 2)
				{
					if (is_sword_spine)
					{
						dreyar->GetModel()->Dettach(weaponSword, 3);

						Transform attachTransform;
						attachTransform.Position(-60, -20, -200);
						attachTransform.Scale(10.0f, 10.0f, 10.0f);

						dreyar->GetModel()->Attach(shader, weaponSword, 36, &attachTransform);

						is_sword_spine = false;
						is_sword = true;

					}
					dreyar->SetWeaponNum(2);
					dreyar->Render3();
				}
			}
		}
		else if (!is_model)
		{

		}
	}
	water->ResetClipPlane();

}

void PortFolio::Render()
{
	if (is_ocean)
	{
		ocean->Render(Time::Get()->Running());
	}

	sky->Pass(4, 5, 6);
	sky->Render();

	if (terrain_num == 1)
	{
		//terrain->Pass(18);
		terrain->Render();
	}
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
		
	}
	else if (!is_mesh)
	{

	}

	if (is_model)
	{
		weaponArrow->Render();
		castleGuardSword->Render2();
		castleGuardBow->Render3();
		if (dreyar != NULL)
		{
			if (particle != NULL)
				particle->Render();			

			if (is_particle_attack_one)
			{
				particle1->Render();
				particle2->Render();
				particle3->Render();
				particle4->Render();
			}

			if (is_particle_attack_two && particleposition.y > -5)
			{
				meteor_particle1->Render();
				meteor_particle2->Render();
				meteor_particle3->Render();
			}

			if (weapon_num == 0)
			{
				if (is_bow)
				{
					dreyar->GetModel()->Dettach(weaponBow, 36);

					is_bow = false;
				}

				dreyar->Render();

				is_unarmed = true;
			}
			else if (weapon_num == 1)
			{
				if (particle5 != NULL && is_trail)
					particle5->Render();

				if (is_unarmed)
				{
					Transform attachTransform;
					attachTransform.Position(-60, -20, -200);
					attachTransform.Scale(10.0f, 10.0f, 10.0f);

					dreyar->GetModel()->Attach(shader, weaponSword, 36, &attachTransform);

					is_unarmed = false;
					is_sword = true;
				}

				dreyar->Render2();
			}
			else if (weapon_num == 2)
			{
				if (is_sword)
				{
					dreyar->GetModel()->Dettach(weaponSword, 36);

					Transform attachTransform;
					attachTransform.Position(-9.0f, -3.0f, 0.0f);
					attachTransform.RotationDegree(90, -90, 150);
					attachTransform.Scale(30.0f, 30.0f, 30.0f);
					dreyar->GetModel()->Attach(shader, weaponBow, 36, &attachTransform);

					is_sword = false;
					is_bow = true;
				}

				dreyar->Render3();
			}
		}

		//충돌
		//Player - MonSword
		for (int i = 0; i < 5; i++)
		{
			is_collision_sword[i] = player_colliders.Collider->IsIntersect(mon_sword_colliders[i].Collider);
			if (is_collision_sword[i] == true)
				is_collision = true;
		}
		//Player - MonArrow
		for (int i = 1; i < 6; i++)
		{
			is_collision_arrow[i] = player_colliders.Collider->IsIntersect(arrow_colliders[i].Collider);
			if (is_collision_arrow[i] == true)
				is_collision = true;
		}
		if (
			is_collision_sword[0] == false && 
			is_collision_sword[1] == false && 
			is_collision_sword[2] == false && 
			is_collision_sword[3] == false && 
			is_collision_sword[4] == false && 												
			is_collision_arrow[1] == false &&	
			is_collision_arrow[2] == false &&	
			is_collision_arrow[3] == false &&	
			is_collision_arrow[4] == false &&	
			is_collision_arrow[5] == false  
			)
			is_collision = false;

		//Playersword - Monbody
		for (int i = 0; i < 10; i++)
		{
			if ((weapon_num == 1 && is_attacking) || (weapon_num == 1 && is_attackcombo_two))
			{
				is_collision_sword_mon[i] = monster_colliders[i].Collider->IsIntersect(player_sword_colliders.Collider);
				if (is_collision_sword_mon[i] == true)
					is_mon_heat[i] = true;
			}
			//Playerarrow - Monbody	
			if (weapon_num == 2 && is_arrow_moving)
			{
				is_collision_arrow_mon[i] = monster_colliders[i].Collider->IsIntersect(arrow_colliders[0].Collider);
				if (is_collision_arrow_mon[i] == true)
					is_mon_heat[i] = true;
			}

			if (is_collision_sword_mon[i] == false && is_collision_arrow_mon[i] == false)
				is_mon_heat[i] = false;
		}

		for (int i = 0; i < 10; i++)
		{
			if ((weapon_num == 0 && is_particle_attack_one))
			{
				Vector3 meshposition;
				sphere->GetTransform(0)->Position(&meshposition);

				if (powf(mon_position[i].x - meshposition.x, 2) + powf(mon_position[i].z - meshposition.z, 2) < 10.0f)
				{
					is_mon_heat[i] = true;
				}
			}

			if ((weapon_num == 0 && is_particle_attack_two))
			{
				Vector3 meshposition;
				sphere->GetTransform(0)->Position(&meshposition);

				if (meshposition.y < 5.0f)
				{
					if (powf(mon_position[i].x - meshposition.x, 2) + powf(mon_position[i].z - meshposition.z, 2) < 10.0f)
					{
						is_mon_heat[i] = true;
					}
				}
			}
		}


		if (is_collision)
		{
			is_heat = true;
		}

		//arrow
		for (int i = 0; i < 6; i++)
		{
			arrow_colliders[i].Collider->Render(Color(0, 1, 0, 1));
		}

		//ImGui::Checkbox("collider render", &is_collider_rendering);
		
		if (is_collider_rendering)
		{
			//충돌체 처리
			player_colliders.Collider->Render(is_collision ? Color(1, 0, 0, 1) : Color(0, 1, 0, 1));

			//player sword
			if (weapon_num == 1)
				player_sword_colliders.Collider->Render(Color(0, 1, 0, 1));


			//monster body
			for (int i = 0; i < 10; i++)
			{
				if (is_mon_death[i] == false)
					monster_colliders[i].Collider->Render(is_mon_heat[i] ? Color(1, 0, 0, 1) : Color(0, 1, 0, 1));
			}

			//mon sword
			for (int i = 0; i < 5; i++)
			{
				if (is_mon_death[i] == false)
					mon_sword_colliders[i].Collider->Render(Color(0, 1, 0, 1));
			}

			

		}

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
	//Vector3 picked = terrainLod->GetPickedPosition();
	//string str = to_string(picked.x) + ", " + to_string(picked.y) + ", " + to_string(picked.z);
	//Gui::Get()->RenderText(Vector2(10, 60), Color(1, 0, 0, 1), "Picked : " + str);

	
	bb->Render();
	bb2->Render();
	bb3->Render();
	bb4->Render();
	bb5->Render();
	bb6->Render();
	bb7->Render();
	bb8->Render();	
}

void PortFolio::PostRender()
{
	//sky->PostRender();
	
	ImGui::Image(shadow->SRV(), ImVec2(200, 200));

	if(is_ocean)
		ocean->RenderFFT();
}

void PortFolio::Mesh()
{
	//Create Material
	{
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

		sphere = new MeshRender(shader, new MeshSphere(0.1f, 20, 20));
		
		transform = sphere->AddTransform();
		transform->Position(128.0f + -30, 0, 128.0f + -15.0f + (float)0 * 15.0f);
		transform->Scale(0.1f, 0.1f, 0.1f);
	}

	sphere->UpdateTransforms();

	meshes.push_back(sphere);
}

void PortFolio::Weapon()
{
	weaponSword = new Model();
	weaponSword->ReadMaterial(L"Weapon/Sword");
	weaponSword->ReadMesh(L"Weapon/Sword");	

	weaponBow = new Model();
	weaponBow->ReadMaterial(L"Weapon/LongBow");
	weaponBow->ReadMesh(L"Weapon/LongBow");

	weaponArrow = new ModelRender(shader);
	weaponArrow->ReadMaterial(L"Weapon/LongArrow");
	weaponArrow->ReadMesh(L"Weapon/LongArrow");
	for (float x = 0; x < 6; x++)
	{
		Transform* transform_arrow = weaponArrow->AddTransform();
		transform_arrow->Position(x - 1000, 0, -1000);
		transform_arrow->RotationDegree(Vector3(0, 0, 90));
		transform_arrow->Scale(0.1f, 0.1f, 0.1f);
	}
	models.push_back(weaponArrow);
	weaponArrow->Render();
}

void PortFolio::Dreyar()
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
	dreyar->ReadClip(L"Dreyar/StandingDrawArrow");			//15
	dreyar->ReadClip(L"Dreyar/StandingAimRecoil");			//16
	dreyar->ReadClip(L"Dreyar/ShootingArrow");				//17
	dreyar->ReadClip(L"Dreyar/StandingDeathRight");			//18

	Transform attachTransform;
	attachTransform.Position(-60, -20, -200);
	attachTransform.Scale(10.0f, 10.0f, 10.0f);

	dreyar->SetAttachBone(36);
	dreyar->SetWeaponNum(0);
	dreyar->Render();

	dreyar->SetWeaponNum(1);
	dreyar->GetModel()->Attach(shader, weaponSword, 36, &attachTransform);
	dreyar->Render2();

	dreyar->SetWeaponNum(2);
	dreyar->GetModel()->Dettach(weaponSword, 36);
	attachTransform.Position(-9.0f, -3.0f, 0.0f);
	attachTransform.RotationDegree(90, -90, 150);
	attachTransform.Scale(30.0f, 30.0f, 30.0f);
	dreyar->GetModel()->Attach(shader, weaponBow, 36, &attachTransform);
	dreyar->Render3();

	dreyar->GetModel()->Dettach(weaponBow, 36);
	dreyar->SetWeaponNum(0);

	Transform* transform = dreyar->AddTransform();
	transform->Position(128.0f, terrain->GetHeight(Vector3(128,0,128)), 128.0f);
	transform->Scale(0.001f, 0.001f, 0.001f);

	dreyar->UpdateTransforms();

	animators.push_back(dreyar);

	//충돌 - Player Body
	{
		player_colliders.Init = new Transform();
		player_colliders.Init->Scale(400, 1600, 400);
		player_colliders.Init->Position(0, 800, 0);

		player_colliders.Transform = new Transform();
		player_colliders.Collider = new Collider(player_colliders.Transform, player_colliders.Init);
	}

	//Player Sword
	{
		player_sword_colliders.Init = new Transform();
		player_sword_colliders.Init->Scale(100, 100, 1200);
		player_sword_colliders.Init->Position(-100, 0, -700);

		player_sword_colliders.Transform = new Transform();
		player_sword_colliders.Collider = new Collider(player_sword_colliders.Transform, player_sword_colliders.Init);
	}

	//Collision - Arrow
	for (int i = 0; i < 6; i++)
	{
		arrow_colliders[i].Init = new Transform();
		arrow_colliders[i].Init->Scale(1, 15, 1);
		arrow_colliders[i].Init->Position(0, 0, 0);

		arrow_colliders[i].Transform = new Transform();
		arrow_colliders[i].Collider = new Collider(arrow_colliders[i].Transform, arrow_colliders[i].Init);
	}
}

void PortFolio::CastleGuardSword()
{
	castleGuardSword = new ModelAnimator(shader);
	castleGuardSword->ReadMaterial(L"CastleGuard/Mesh");
	castleGuardSword->ReadMesh(L"CastleGuard/Mesh");
	castleGuardSword->ReadClip(L"CastleGuard/Idle");					//0
	castleGuardSword->ReadClip(L"CastleGuard/Running");					//1
	castleGuardSword->ReadClip(L"CastleGuard/Attacking");				//2
	castleGuardSword->ReadClip(L"CastleGuard/StandingReactLeft");		//3
	castleGuardSword->ReadClip(L"CastleGuard/ShootingArrow");			//4 - 150frame
	castleGuardSword->ReadClip(L"CastleGuard/StandingDeathBackward");	//5 - 77frame

	Transform attachTransform;
	attachTransform.Position(-10, 0, -10);
	attachTransform.Scale(1.0f, 1.0f, 1.0f);
	
	castleGuardSword->SetAttachBone(23);
	castleGuardSword->SetWeaponNum(1);
	castleGuardSword->GetModel()->Attach(shader, weaponSword, 23, &attachTransform);
	castleGuardSword->Render2();
	castleGuardSword->CreateComputeDesc();

	std::mt19937 engine3((unsigned int)time(NULL));               // MT19937 난수 엔진
	std::uniform_real_distribution<float> distribution3(0.0f, 20.0f);       // 생성 범위
	auto generator3 = bind(distribution3, engine3);

	Transform* transform = NULL;	

	for (UINT i = 0; i < 5; i++)
	{
		transform = castleGuardSword->AddTransform();
		transform->Position(120.0f + generator3(), 0, 120.0f + generator3());
		transform->Scale(0.01f, 0.01f, 0.01f);
	}

	castleGuardSword->UpdateTransforms();

	animators.push_back(castleGuardSword);

	//충돌처리
	for (UINT i = 0; i < 5; i++)
	{
		mon_sword_colliders[i].Init = new Transform();
		mon_sword_colliders[i].Init->Scale(10, 10, 120);
		mon_sword_colliders[i].Init->Position(-10, 0, -60);

		mon_sword_colliders[i].Transform = new Transform();
		mon_sword_colliders[i].Collider = new Collider(mon_sword_colliders[i].Transform, mon_sword_colliders[i].Init);
	}
}

void PortFolio::CastleGuardBow()
{
	castleGuardBow = new ModelAnimator(shader);
	castleGuardBow->ReadMaterial(L"CastleGuard/Mesh");
	castleGuardBow->ReadMesh(L"CastleGuard/Mesh");
	castleGuardBow->ReadClip(L"CastleGuard/Idle");						//0
	castleGuardBow->ReadClip(L"CastleGuard/Running");					//1
	castleGuardBow->ReadClip(L"CastleGuard/Attacking");					//2
	castleGuardBow->ReadClip(L"CastleGuard/StandingReactLeft");			//3
	castleGuardBow->ReadClip(L"CastleGuard/ShootingArrow");				//4
	castleGuardBow->ReadClip(L"CastleGuard/StandingDeathBackward");		//5

	Transform attachTransform;
	attachTransform.Position(-9.0f, -3.0f, 0.0f);
	attachTransform.RotationDegree(90, -90, 150);
	attachTransform.Scale(5.0f, 5.0f, 5.0f);

	castleGuardBow->SetWeaponNum(2);
	castleGuardBow->GetModel()->Attach(shader, weaponBow, 23, &attachTransform);
	castleGuardBow->Render3();
	//castleGuardBow->CreateComputeDesc();

	//castleGuardBow->Pass(6);

	Transform* transform = NULL;

	std::mt19937 engine3((unsigned int)time(NULL));               // MT19937 난수 엔진
	std::uniform_real_distribution<float> distribution3(0.0f, 20.0f);       // 생성 범위
	auto generator3 = bind(distribution3, engine3);

	for (UINT i = 0; i < 5; i++)
	{
		transform = castleGuardBow->AddTransform();
		transform->Position(120.0f + generator3(), 5.0f, 120.0f + generator3());
		transform->Scale(0.01f, 0.01f, 0.01f);
	}

	castleGuardBow->UpdateTransforms();

	animators.push_back(castleGuardBow);

	//충돌처리
	for (UINT i = 0; i < 10; i++)
	{
		monster_colliders[i].Init = new Transform();
		monster_colliders[i].Init->Scale(40, 180, 40);
		monster_colliders[i].Init->Position(0, 100, 0);
		monster_colliders[i].Transform = new Transform();
		monster_colliders[i].Collider = new Collider(monster_colliders[i].Transform, monster_colliders[i].Init);
	}
}

void PortFolio::BillboardLayer()
{
	ImGui::SliderInt("Tree Layer", &treenum, 0, 7);
	
	ImVec2 aa = ImGui::GetMousePos();

	if (aa.x >= 0.0f && aa.x < 362.0f && aa.y > 250.0f && aa.y <= 620)
		is_billboard_hovered = true;
	else
		is_billboard_hovered = false;

	if (ImGui::ImageButton(tex1->SRV(), ImVec2(70, 70)))
		treenum = 0;
	ImGui::SameLine();
	if (ImGui::ImageButton(tex2->SRV(), ImVec2(70, 70)))
		treenum = 1;
	ImGui::SameLine();
	if (ImGui::ImageButton(tex3->SRV(), ImVec2(70, 70)))
		treenum = 2;
	ImGui::SameLine();
	if (ImGui::ImageButton(tex4->SRV(), ImVec2(70, 70)))
		treenum = 3;

	if (ImGui::ImageButton(tex5->SRV(), ImVec2(70, 70)))
		treenum = 4;
	ImGui::SameLine();
	if (ImGui::ImageButton(tex6->SRV(), ImVec2(70, 70)))
		treenum = 5;
	ImGui::SameLine();
	if (ImGui::ImageButton(tex7->SRV(), ImVec2(70, 70)))
		treenum = 6;
	ImGui::SameLine();
	if (ImGui::ImageButton(tex8->SRV(), ImVec2(70, 70)))
		treenum = 7;


	if (!is_billboard_hovered)
	{
		if (Mouse::Get()->Press(0))
		{
			auto picked = terrain->GetPickedPosition();
			for (UINT i = 0; i < 10; i++)
			{
				Vector2 scale = Math::RandomVec2(1.0f, 3.0f);
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

	if (ImGui::Button("BillboardSave"))
	{
		std::wstring file;
		Path::SaveFileDialog(file, Path::ImageFilter, L"../../Textures/Save/Billboard/", [=](std::wstring path) {

			auto writer = new BinaryWriter();
			writer->Open(path, 1);

			writer->Int(bb->GetSize());
			writer->Int(bb2->GetSize());
			writer->Int(bb3->GetSize());
			writer->Int(bb4->GetSize());
			writer->Int(bb5->GetSize());
			writer->Int(bb6->GetSize());
			writer->Int(bb7->GetSize());
			writer->Int(bb8->GetSize());

			if (bb->GetSize() > 0)
			{
				for (int i = 0; i < bb->GetSize(); i++)
				{
					Vector3 saveposition = bb->GetPosition(i);
					Vector2 savescale = bb->GetScale(i);
					float saverandom = bb->GetRandom(i);

					writer->Vector3(saveposition);
					writer->Vector2(savescale);
					writer->Float(saverandom);
				}
			}

			if (bb2->GetSize() > 0)
			{
				for (int i = 0; i < bb2->GetSize(); i++)
				{
					Vector3 saveposition = bb2->GetPosition(i);
					Vector2 savescale = bb2->GetScale(i);
					float saverandom = bb2->GetRandom(i);

					writer->Vector3(saveposition);
					writer->Vector2(savescale);
					writer->Float(saverandom);
				}
			}

			if (bb3->GetSize() > 0)
			{
				for (int i = 0; i < bb3->GetSize(); i++)
				{
					Vector3 saveposition = bb3->GetPosition(i);
					Vector2 savescale = bb3->GetScale(i);
					float saverandom = bb3->GetRandom(i);

					writer->Vector3(saveposition);
					writer->Vector2(savescale);
					writer->Float(saverandom);
				}
			}

			if (bb4->GetSize() > 0)
			{
				for (int i = 0; i < bb4->GetSize(); i++)
				{
					Vector3 saveposition = bb4->GetPosition(i);
					Vector2 savescale = bb4->GetScale(i);
					float saverandom = bb4->GetRandom(i);

					writer->Vector3(saveposition);
					writer->Vector2(savescale);
					writer->Float(saverandom);
				}
			}

			if (bb5->GetSize() > 0)
			{
				for (int i = 0; i < bb5->GetSize(); i++)
				{
					Vector3 saveposition = bb5->GetPosition(i);
					Vector2 savescale = bb5->GetScale(i);
					float saverandom = bb5->GetRandom(i);

					writer->Vector3(saveposition);
					writer->Vector2(savescale);
					writer->Float(saverandom);
				}
			}

			if (bb6->GetSize() > 0)
			{
				for (int i = 0; i < bb6->GetSize(); i++)
				{
					Vector3 saveposition = bb6->GetPosition(i);
					Vector2 savescale = bb6->GetScale(i);
					float saverandom = bb6->GetRandom(i);

					writer->Vector3(saveposition);
					writer->Vector2(savescale);
					writer->Float(saverandom);
				}
			}

			if (bb7->GetSize() > 0)
			{
				for (int i = 0; i < bb7->GetSize(); i++)
				{
					Vector3 saveposition = bb7->GetPosition(i);
					Vector2 savescale = bb7->GetScale(i);
					float saverandom = bb7->GetRandom(i);

					writer->Vector3(saveposition);
					writer->Vector2(savescale);
					writer->Float(saverandom);
				}
			}

			if (bb8->GetSize() > 0)
			{
				for (int i = 0; i < bb->GetSize(); i++)
				{
					Vector3 saveposition = bb8->GetPosition(i);
					Vector2 savescale = bb8->GetScale(i);
					float saverandom = bb8->GetRandom(i);

					writer->Vector3(saveposition);
					writer->Vector2(savescale);
					writer->Float(saverandom);
				}
			}

			writer->Close();
		});
	}
	ImGui::SameLine();
	if (ImGui::Button("BillboardLoad"))
	{
		std::wstring file;
		Path::OpenFileDialog(file, Path::ImageFilter, L"../../Textures/Save/Billboard/", [=](std::wstring path) {
			auto reader = new BinaryReader();

			reader->Open(path);
			int bbsize = reader->Int();
			int bb2size = reader->Int();
			int bb3size = reader->Int();
			int bb4size = reader->Int();
			int bb5size = reader->Int();
			int bb6size = reader->Int();
			int bb7size = reader->Int();
			int bb8size = reader->Int();

			if (bbsize != 0)
			{
				for (int i = 0; i < bbsize; i++)
				{
					Vector3 saveposition = reader->Vector3();
					Vector2 savescale = reader->Vector2();
					float saverandom = reader->Float();

					bb->Add(saveposition, savescale, saverandom);
				}
			}

			if (bb2size != 0)
			{
				for (int i = 0; i < bb2size; i++)
				{
					Vector3 saveposition = reader->Vector3();
					Vector2 savescale = reader->Vector2();
					float saverandom = reader->Float();

					bb2->Add(saveposition, savescale, saverandom);
				}
			}

			if (bb3size != 0)
			{
				for (int i = 0; i < bb3size; i++)
				{
					Vector3 saveposition = reader->Vector3();
					Vector2 savescale = reader->Vector2();
					float saverandom = reader->Float();

					bb3->Add(saveposition, savescale, saverandom);
				}
			}

			if (bb4size != 0)
			{
				for (int i = 0; i < bb4size; i++)
				{
					Vector3 saveposition = reader->Vector3();
					Vector2 savescale = reader->Vector2();
					float saverandom = reader->Float();

					bb4->Add(saveposition, savescale, saverandom);
				}
			}

			if (bb5size != 0)
			{
				for (int i = 0; i < bb5size; i++)
				{
					Vector3 saveposition = reader->Vector3();
					Vector2 savescale = reader->Vector2();
					float saverandom = reader->Float();

					bb5->Add(saveposition, savescale, saverandom);
				}
			}

			if (bb6size != 0)
			{
				for (int i = 0; i < bb6size; i++)
				{
					Vector3 saveposition = reader->Vector3();
					Vector2 savescale = reader->Vector2();
					float saverandom = reader->Float();

					bb6->Add(saveposition, savescale, saverandom);
				}
			}

			if (bb7size != 0)
			{
				for (int i = 0; i < bb7size; i++)
				{
					Vector3 saveposition = reader->Vector3();
					Vector2 savescale = reader->Vector2();
					float saverandom = reader->Float();

					bb7->Add(saveposition, savescale, saverandom);
				}
			}

			if (bb8size != 0)
			{
				for (int i = 0; i < bb8size; i++)
				{
					Vector3 saveposition = reader->Vector3();
					Vector2 savescale = reader->Vector2();
					float saverandom = reader->Float();

					bb8->Add(saveposition, savescale, saverandom);
				}
			}

			reader->Close();
		});
	}
}

void PortFolio::Pass(UINT mesh, UINT model, UINT anim)
{
	for (MeshRender* temp : meshes)
		temp->Pass(mesh);

	for (ModelRender* temp : models)
		temp->Pass(model);

	for (ModelAnimator* temp : animators)
		temp->Pass(anim);
}

void PortFolio::AddPointLights()
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

void PortFolio::AddSpotLights()
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