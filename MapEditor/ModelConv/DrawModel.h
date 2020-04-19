#pragma once
#include "Systems/IExecute.h"

class DrawModel : public IExecute
{
	virtual void Initialize() override;
	virtual void Ready() {}
	virtual void Destroy() {}
	virtual void Update() override;
	virtual void PreRender() {}
	virtual void Render() override;
	virtual void PostRender() {}
	virtual void ResizeScreen() {}

private:
	void Airplane();
	void Tower();
	void Tank();
	void Stones();
	void Tree();

private:
	Shader* shader;

	ModelRender* airplane = NULL;
	ModelRender* tower = NULL;
	ModelRender* tank = NULL;
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

};