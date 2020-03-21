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
	void CastleGuard();

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
	ModelAnimator* dreyar = NULL;
	ModelAnimator* castleGuard = NULL;

	bool is_unarmed = false;
	bool is_sword_spine = false;
	bool is_sword = false;
	int weapon_num = 0;
	UINT playerClip = 0;
	float speed = 1.3f;
	float taketime = 0.2f;

	struct ColliderDesc
	{
		Transform* Init;
		Transform* Transform;
		Collider* Collider;
	} colliders[4];
	

	vector<MeshRender *> meshes;
	vector<ModelRender *> models;
	vector<ModelAnimator *> animators;
};