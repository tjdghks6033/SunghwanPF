#include "Framework.h"
#include "OrbitCamera.h"

OrbitCamera::OrbitCamera(Vector3* targetPos)
	:Camera(), target(targetPos)
{
}

OrbitCamera::~OrbitCamera()
{
}

void OrbitCamera::Update()
{
	Vector3 R;
	Rotation(&R);

	Vector3 P;

	P.x = sin(R.y) * cos(R.x);
	P.y = sin(-R.x);
	P.z = cos(R.y) * cos(R.x);

	Position(*target - (P * distance));

	if (Mouse::Get()->Press(2))
	{
		Vector3 val = Mouse::Get()->GetMoveValue();

		distance += 20.0f * val.y * Time::Delta();
	}

	//Rotation
	if (Mouse::Get()->Press(1) == false) return;

	{
		Vector3 val = Mouse::Get()->GetMoveValue();
		R.x = R.x + (val.y * rotation * Time::Delta());
		R.y = R.y + (val.x * rotation * Time::Delta());
		R.z = 0.0f;

		if (R.x > Math::ToRadian(90))
			R.x = Math::ToRadian(90);
		else if (R.x < Math::ToRadian(0))
			R.x = Math::ToRadian(0);

		if (R.y >= Math::ToRadian(360))
			R.y -= Math::ToRadian(360);
		else if (R.y < 0)
			R.y += Math::ToRadian(360);

		Rotation(R);
	}



}

void OrbitCamera::Speed(float move, float rotation)
{
	this->move = move;
	this->rotation = rotation;
}
