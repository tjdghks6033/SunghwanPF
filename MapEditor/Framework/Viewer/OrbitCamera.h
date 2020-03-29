#pragma once
#include "Camera.h"

class OrbitCamera : public Camera
{
public:
	OrbitCamera(Vector3* targetPos);
	~OrbitCamera();

	void Update() override;

	void Speed(float move, float rotation = 2.0f);

private:
	float move = 10.0f;
	float rotation = 2.0f;

	float distance = 10.0f;

	Vector3* target;
};
