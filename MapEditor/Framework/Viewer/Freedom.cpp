#include "Framework.h"
#include "Freedom.h"

Freedom::Freedom()
	: Camera()
{

}

Freedom::~Freedom()
{

}

void Freedom::Update()
{
	if (!is_orbit)
	{
		if (Mouse::Get()->Press(1) == false) return;

		Vector3 f = Forward();
		Vector3 u = Up();
		Vector3 r = Right();

		//Move
		{
			Vector3 P;
			Position(&P);

			if (Keyboard::Get()->Press('W'))
				P = P + f * move * Time::Delta();
			else if (Keyboard::Get()->Press('S'))
				P = P - f * move * Time::Delta();

			if (Keyboard::Get()->Press('D'))
				P = P + r * move * Time::Delta();
			else if (Keyboard::Get()->Press('A'))
				P = P - r * move * Time::Delta();

			if (Keyboard::Get()->Press('E'))
				P = P + u * move * Time::Delta();
			else if (Keyboard::Get()->Press('Q'))
				P = P - u * move * Time::Delta();

			Position(P);
		}

		//Rotation
		{
			Vector3 R;
			Rotation(&R);

			Vector3 val = Mouse::Get()->GetMoveValue();
			R.x = R.x + val.y * rotation * Time::Delta();
			R.y = R.y + val.x * rotation * Time::Delta();
			R.z = 0.0f;

			Rotation(R);
		}
	}
	else if (is_orbit)
	{
		Vector3 R;
		Rotation(&R);

		Vector3 P;

		P.x = sin(R.y) * cos(R.x);
		P.y = sin(-R.x);
		P.z = cos(R.y) * cos(R.x);

		Position(target - (P * distance));

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

}

void Freedom::Speed(float move, float rotation)
{
	this->move = move;
	this->rotation = rotation;
}
