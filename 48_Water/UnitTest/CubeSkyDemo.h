#pragma once
#include "Systems/IExecute.h"

class CubeSkyDemo : public IExecute 
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	Shader* shader;

	MeshRender* cube;
	MeshRender* cylinder;
	MeshRender* sphere;

	MeshRender* grid;

	Shader* terrainShader;
	class CubeSky* sky;
	class Terrain* terrain;
};