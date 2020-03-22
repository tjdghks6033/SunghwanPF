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
	//void Kachujin();
	void Dreyar();
	void CastleGuardSword();
	void CastleGuardBow();


	void BillboardLayer();

	void Pass(UINT mesh, UINT model, UINT anim);

	void AddPointLights();
	void AddSpotLights();

private:
	Shader* shader;

	Shadow* shadow;

	Terrain* terrain;
	Shader* sTerrain;
	Shader* sTerrainLod;
	Shader* sOcean;

	TerrainLod* terrainLod;

	UINT terrain_num = 0;
	bool is_billboard = false;
	bool is_wireframe = false;
	bool is_mesh = false;
	bool is_model = false;
	bool is_weather = true;
	bool is_light = false;
	bool is_water = false;
	bool is_ocean = false;

	Shader* bbShader;
	class Billboard* bb;
	class Billboard* bb2;
	class Billboard* bb3;
	class Billboard* bb4;
	class Billboard* bb5;
	class Billboard* bb6;
	class Billboard* bb7;
	class Billboard* bb8;
	int treenum = 0;

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

	Model* weaponSword;
	Model* weaponBow;
	ModelRender* weaponArrow;
	ModelAnimator* dreyar = NULL;
	ModelAnimator* castleGuardSword = NULL;
	ModelAnimator* castleGuardBow = NULL;

	bool is_unarmed = false;
	bool is_sword_spine = false;
	bool is_sword = false;	
	bool is_bow = false;
	
	//Player
	Vector3 picked;						//마우스 위치
	float speed;						//플레이어 스피드
	float pickrotation = 0.0f;			//마우스 위치에 대한 회전
	UINT playerClip = 0;				//플레이어클립
	int player_prev_clip = 0;			//플레이어 이전 클립
	int weapon_num = 0;					//플레이어 무기교체
	bool is_attacking = false;			//플레이어가 공격하는지 확인
	bool is_running = false;			//플레이어가 뛰고 있는지 확인
	bool is_sword_attack = false;		//플레이어가 범위 공격하는지 확인
	bool is_sword_jump_attack = false;	//플레이어가 대쉬 공격하는지 확인
	bool is_heat = false;				//플레이어가 맞았는지 확인
	bool is_death = false;				//플레이어가 죽었는지 확인
	float animspeed = 1.3f;
	float taketime = 0.2f;

	//Monster
	int clip[10]; //몬스터 애니매이션 변수
	float mon_speed[10]; //몬스터 패트롤 스피드

	bool is_mon_attack[10];  //몬스터가 공격하는지 확인
	bool is_mon_patrol[10];  //몬스터가 패트롤중인지 확인
	bool is_mon_heat[10];	 //몬스터가 맞는중인지 확인
	bool is_mon_death[10];	 //몬스터가 죽었는지 확인
	bool is_mon_running[10]; //몬스터가 뛰고있는지 확인
	bool is_mon_running_to_waypoint[10]; //몬스터가 거점으로 뛰고 있는지 확인
	float monanimspeed = 1.3f;
	float montaketime = 0.2f;
	float monattackrange = 10.0f;
	float monbowattackrange = 100.0f;
	float montracerange = 200.0f;
	float monwaybackrange = 300.0f;

	Vector3 mon_waypoint[10];  //몬스터 거점
	Vector3 mon_position[10];  //몬스터 위치
	Vector3 mon_rotation[10];  //몬스터 방향

	float mon_hp[10];

	int mon_direction[10];	   //몬스터 방향 4가지
	
	bool is_arrow_initialize = false;		//화살의 초깃값 셋팅 했는지
	bool is_arrow_moving = false;			//화살이 움직이고 있는지

	//Collision
	bool is_collision = false;			//플레이어가 충돌중인지
	bool is_collision_sword[20];		//몬스터의 검과 충돌중인지
	bool is_collision_arrow[20];		//활과 충돌중인지

	bool is_collision_sword_mon[20];
	bool is_collision_arrow_mon[20];

	//몬스터 화살
	bool is_monarrow_initialize[5];
	bool is_monarrow_moving[5];

	Vector3 arrow_position = Vector3(0, 0, 0);
	Vector3 arrow_rotation = Vector3(0, 0, 0);

	Vector3 monarrow_position[10];
	Vector3 monarrow_rotation[10];



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
	} arrow_colliders[40];

	vector<MeshRender *> meshes;
	vector<ModelRender *> models;
	vector<ModelAnimator *> animators;
};