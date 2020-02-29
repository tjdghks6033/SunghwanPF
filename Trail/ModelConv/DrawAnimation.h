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

private:
	Shader* shader;
	Trail* trailor[64];
	Model* weapon;
	ModelAnimator* kachujin = NULL;

	Vector3 position2 = Vector3(0,0,0);
	Vector3 scale2 = Vector3(0, 0, 0);
	Vector3 rotation2 = Vector3(0, 0, 0);
};