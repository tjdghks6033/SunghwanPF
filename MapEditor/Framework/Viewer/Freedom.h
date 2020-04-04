#pragma once
#include "Camera.h"

class Freedom : public Camera
{
public:
	Freedom();
	~Freedom();

	void Update() override;
	void Speed(float move, float rotation);
	void SetOrbitCamera(bool val) { this->is_orbit = val; }
	void SetTarget(Vector3 targetPos) { this->target = targetPos; }

private:
	float move = 20.0f;
	float rotation = 2.0f;

	float theta = 0.0f;
	float phi = 0.0f;

	bool is_orbit = false;


	float distance = 10.0f;
	Vector3 target;
};
