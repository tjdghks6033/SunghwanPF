#pragma once
#include "Systems/IExecute.h"

class PortFolio : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void ResizeScreen() override {}

private:
	void Mesh();
	void Weapon();
	void Dreyar();
	void CastleGuardSword();
	void CastleGuardBow();
	void ModelTowerTreeStones();

	void BillboardLayer();

	void Pass(UINT mesh, UINT model, UINT anim);

	void AddPointLights();
	void AddSpotLights();

private:
	Shader* shader;

	Shadow* shadow;

	Terrain* terrain;
	Shader* sOcean;

	TerrainLod* terrainLod;


	float cameraLatitude = 0.0f, cameraLongitude = -1.85f, orbitDistance = 20.0f;
	//Vector3 camDir, camPos, targetPos;	
	Vector3 cameraposition = Vector3(0, 0, 0);
	Vector3 camerarotation = Vector3(0, 0, 0);
	

	UINT terrain_num = 1;
	bool is_billboard = false;
	bool is_wireframe = false;
	bool is_mesh = false;
	bool is_model = false;
	bool is_tree = false;
	bool is_stones = false;
	bool is_tower = false;
	bool is_weather = true;
	bool is_light = false;
	bool is_water = true;
	bool is_ocean = false;
	bool is_sky = true;

	class Billboard* bb;
	class Billboard* bb2;
	class Billboard* bb3;
	class Billboard* bb4;
	class Billboard* bb5;
	class Billboard* bb6;
	class Billboard* bb7;
	class Billboard* bb8;
	int treenum = 0;
	bool is_billboard_hovered = false;

	Texture* tex1;
	Texture* tex2;
	Texture* tex3;
	Texture* tex4;
	Texture* tex5;
	Texture* tex6;
	Texture* tex7;
	Texture* tex8;


	Sky* sky;
	Snow* snow;
	Rain* rain;
	Water* water;
	FFTOcean* ocean;

	enum class Weather
	{
		None = 0, Rain, Snow,
	} weather;

	Material* wall;
	MeshRender* sphere;
	MeshRender* sphere2;

	Model* weaponSword;
	Model* weaponBow;
	ModelRender* weaponArrow;
	ModelAnimator* dreyar = NULL;
	ModelAnimator* castleGuardSword = NULL;
	ModelAnimator* castleGuardBow = NULL;
	
	ModelRender* tower = NULL;
	ModelRender* stone1 = NULL;
	ModelRender* stone2 = NULL;
	ModelRender* stone3 = NULL;
	ModelRender* stone4 = NULL;
	ModelRender* stone5 = NULL;
	ModelRender* tree1 = NULL;
	ModelRender* tree2 = NULL;
	ModelRender* tree3 = NULL;
	ModelRender* tree4 = NULL;
	ModelRender* tree5 = NULL;

	ParticleSystem* particle = NULL;
	ParticleSystem* particle1 = NULL;
	ParticleSystem* particle2 = NULL;
	ParticleSystem* particle3 = NULL;
	ParticleSystem* particle4 = NULL;
	ParticleSystem* particle5 = NULL;

	ParticleSystem* meteor_particle1 = NULL;
	ParticleSystem* meteor_particle2 = NULL;
	ParticleSystem* meteor_particle3 = NULL;

	ParticleSystem* blood_particle = NULL;
	ParticleSystem* blood_particle1 = NULL;
	ParticleSystem* blood_particle2 = NULL;
	ParticleSystem* blood_particle3 = NULL;
	ParticleSystem* blood_particle4 = NULL;
	ParticleSystem* blood_particle5 = NULL;
	ParticleSystem* blood_particle6 = NULL;
	ParticleSystem* blood_particle7 = NULL;
	ParticleSystem* blood_particle8 = NULL;
	ParticleSystem* blood_particle9 = NULL;
	ParticleSystem* blood_particle10 = NULL;

	ParticleSystem* fire_particle1 = NULL;
	ParticleSystem* fire_particle2 = NULL;
	ParticleSystem* fire_particle3 = NULL;
	ParticleSystem* fire_particle4 = NULL;
	ParticleSystem* fire_particle5 = NULL;
	ParticleSystem* fire_particle6 = NULL;
	ParticleSystem* fire_particle7 = NULL;
	ParticleSystem* fire_particle8 = NULL;
	ParticleSystem* fire_particle9 = NULL;
	ParticleSystem* fire_particle10 = NULL;

	ParticleSystem* smoke_particle1 = NULL;
	ParticleSystem* smoke_particle2 = NULL;
	ParticleSystem* smoke_particle3 = NULL;
	ParticleSystem* smoke_particle4 = NULL;
	ParticleSystem* smoke_particle5 = NULL;
	ParticleSystem* smoke_particle6 = NULL;
	ParticleSystem* smoke_particle7 = NULL;
	ParticleSystem* smoke_particle8 = NULL;
	ParticleSystem* smoke_particle9 = NULL;
	ParticleSystem* smoke_particle10 = NULL;

	Vector3 particleposition = Vector3(0, 0, 0);
	

	bool is_unarmed = false;
	bool is_sword_spine = false;
	bool is_sword = false;
	bool is_bow = false;
	
	//Player
	Vector3 picked;							//마우스 위치
	float speed;							//플레이어 스피드
	float pickrotation = 0.0f;				//마우스 위치에 대한 회전
	UINT playerClip = 0;					//플레이어클립
	int player_prev_clip = 0;				//플레이어 이전 클립
	int weapon_num = 0;						//플레이어 무기교체
	bool is_attacking = false;				//플레이어가 공격하는지 확인
	bool is_running = false;				//플레이어가 뛰고 있는지 확인
	bool is_attackcombo_one = false;		//플레이어가 범위 공격하는지 확인
	bool is_attackcombo_two = false;		//플레이어가 대쉬 공격하는지 확인
	bool is_particle_attack_one = false;	//플레이어가 스킬 공격하는지 확인
	bool is_particle_attack_once = false;
	bool is_particle_attack_twice = false;
	bool is_particle_attack_two = false;
	bool is_trail = false;
	bool is_heat = false;					//플레이어가 맞았는지 확인
	bool is_death = false;					//플레이어가 죽었는지 확인
	bool is_blood = false;
	float animspeed = 1.3f;
	float taketime = 0.2f;
	float sword_jump_attack_range = 100.0f;

	//Monster
	int clip[10]; //몬스터 애니매이션 변수
	float mon_speed[10]; //몬스터 패트롤 스피드

	bool is_mon_attack[10];  //몬스터가 공격하는지 확인
	bool is_mon_patrol[10];  //몬스터가 패트롤중인지 확인
	bool is_mon_heat[10];	 //몬스터가 맞는중인지 확인
	bool is_mon_death[10];	 //몬스터가 죽었는지 확인
	bool is_mon_running[10]; //몬스터가 뛰고있는지 확인
	bool is_mon_running_to_waypoint[10]; //몬스터가 거점으로 뛰고 있는지 확인
	bool is_mon_blood[10];
	bool is_mon_fire[10];
	float monanimspeed = 1.3f;
	float montaketime = 0.2f;
	float monattackrange = 3.0f;
	float monbowattackrange = 100.0f;
	float montracerange = 150.0f;
	float monwaybackrange = 2000.0f;

	float arrowposy = 1.318f;
	float monarrowposy = 1.318f;

	Vector3 mon_waypoint[10];  //몬스터 거점
	Vector3 mon_position[10];  //몬스터 위치
	Vector3 mon_rotation[10];  //몬스터 방향

	float mon_hp[10];

	int mon_direction[10];	   //몬스터 방향 4가지
	
	bool is_arrow_initialize = false;		//화살의 초깃값 셋팅 했는지
	bool is_arrow_moving = false;			//화살이 움직이고 있는지

	//Collision
	bool is_collision = false;			//플레이어가 충돌중인지
	bool is_collision_sword[5];		//몬스터의 검과 충돌중인지
	bool is_collision_arrow[5];		//활과 충돌중인지

	bool is_collision_sword_mon[10];
	bool is_collision_arrow_mon[10];

	//몬스터 화살
	bool is_monarrow_initialize[5];
	bool is_monarrow_moving[5];

	Vector3 arrow_position = Vector3(0, 0, 0);
	Vector3 arrow_rotation = Vector3(0, 0, 0);

	Vector3 monarrow_position[5];
	Vector3 monarrow_rotation[5];

	bool is_collider_rendering = false;

	struct PlayerColliderDesc
	{
		Transform* Init;
		Transform* Transform;
		Collider* Collider;
	} player_colliders;
	struct PlayerSwordColliderDesc
	{
		Transform* Init;
		Transform* Transform;
		Collider* Collider;
	} player_sword_colliders;				//player - sword, arrow	= 2

	struct MonsterColliderDesc
	{
		Transform* Init;
		Transform* Transform;
		Collider* Collider;
	} monster_colliders[10];

	struct MonsterSwordColliderDesc
	{
		Transform* Init;
		Transform* Transform;
		Collider* Collider;
	} mon_sword_colliders[5];

	struct ArrowColliderDesc
	{
		Transform* Init;
		Transform* Transform;
		Collider* Collider;
	} arrow_colliders[6];

	vector<MeshRender *> meshes;
	vector<ModelRender *> models;
	vector<ModelAnimator *> animators;
};