#pragma once
#include "Systems/IExecute.h"

class DrawAnimation : public IExecute
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
	void Kachujin();
	void Dreyar();

private:
	Shader* shader;

	Model* weapon;
	ModelAnimator* kachujin = NULL;
	ModelAnimator* dreyar = NULL;
};