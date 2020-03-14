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
	void Amy();
	void CastleGuard();
	void Dreyar();
	void Ganfaul();

private:
	Shader* shader;
	Trail* trailor[64];
	Model* weapon;
	ModelAnimator* kachujin = NULL;
	ModelAnimator* amy = NULL;
	ModelAnimator* castleGuard = NULL;
	ModelAnimator* dreyar = NULL;
	ModelAnimator* ganfaul = NULL;

	Vector3 position2 = Vector3(0,0,0);
	Vector3 scale2 = Vector3(0, 0, 0);
	Vector3 rotation2 = Vector3(0, 0, 0);

	struct ColliderDesc
	{
		Transform* Init;
		Transform* Transform;
		Collider* Collider;
	} colliders[4];
};