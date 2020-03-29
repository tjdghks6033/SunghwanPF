#pragma once

#pragma region Light
#define MAX_SPOT_LIGHT 128
#define MAX_POINT_LIGHT 128

struct PointLight
{
	Color Ambient;
	Color Diffuse;
	Color Specular;
	Color Emissive;

	Vector3 Position;
	float Range;

	float Intensity;
	float Padding[3];
};

struct SpotLight
{
	Color Ambient;
	Color Diffuse;
	Color Specular;
	Color Emissive;

	Vector3 Position;
	float Range;

	Vector3 Direction;
	float Angle;

	float Intensity;
	float Padding[3];
};
#pragma endregion

class Context
{
public:
	static Context* Get();
	static void Create();
	static void Delete();

private:
	Context();
	~Context();

public:
	void ResizeScreen();

	void Update();
	void Render();

	Matrix View();
	Matrix Projection();

	class Perspective* GetPerspective() { return perspective; }
	class Viewport* GetViewport() { return viewport; }
	class Camera* GetCamera() { return camera; }

	void NewOrbitCamera(Vector3* pos);

	Color& Ambient() { return ambient; }
	Color& Specular() { return specular; }
	Vector3& Direction() { return direction; }
	Vector3& Position() { return position; }

	UINT PointLights(OUT PointLight* lights);
	void AddPointLight(PointLight& light);
	PointLight& GetPointLight(UINT index);

	UINT SpotLights(OUT SpotLight* lights);
	void AddSpotLight(SpotLight& light);
	SpotLight& GetSpotLight(UINT index);

	Color& FogColor() { return fogColor; }
	Vector2& FogDistance() { return fogDistance; }
	float& FogDensity() { return fogDensity; }
	UINT& FogType() { return fogType; }

private:
	static Context* instance;

private:
	class Perspective* perspective;
	class Viewport* viewport;
	class Camera* camera;

	Color ambient = Color(0, 0, 0, 1);
	Color specular = Color(1, 1, 1, 1);
	Vector3 direction = Vector3(-1, -1, 1);
	Vector3 position = Vector3(0, 0, 0);


	UINT pointLightCount = 0;
	PointLight pointLights[MAX_POINT_LIGHT];

	UINT spotLightCount = 0;
	SpotLight spotLights[MAX_SPOT_LIGHT];


	Color fogColor = Color(1, 1, 1, 1);
	Vector2 fogDistance = Vector2(1, 105);
	float fogDensity = 0.5f;
	UINT fogType = 2;
};