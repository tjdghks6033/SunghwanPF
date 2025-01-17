#pragma once

class Sky
{
public:
	Sky(Shader* shader);
	~Sky();
	
	void ScatteringPass(UINT val);
	void Pass(UINT domePass, UINT moonPass, UINT cloudPass);

	void Update();
	void PreRender();
	void Render();
	void PostRender();

	void RealTime(bool val, float theta, float timeFactor = 1.0f);


private:
	struct ScatterDesc
	{
		Vector3 WaveLength = Vector3(0.65f, 0.57f, 0.475f);
		float Padding;

		Vector3 InvWaveLength;
		int SampleCount = 8;

		Vector3 WaveLengthMie;
		float Padding2;
	} scatterDesc;

	struct CloudDesc
	{
		float Tiles = 1.5f;
		float Cover = 0.005f;
		float Sharpness = 0.405f;
		float Speed = 0.05f;
	} cloudDesc;

	struct PerlinCloudDesc
	{
		float translation = 0.0f;
		float scale = 0.3f;
		float brightness = 0.5f;
		float padding;
	} perlincloudDesc;

private:
	Shader* shader;

	bool bRealTime = false;

	float timeFactor = 1.0f;
	float theta = 0.0f;

	class Scattering* scattering;
	ConstantBuffer* scatterBuffer;
	ID3DX11EffectConstantBuffer* sScatterBuffer;

	ID3DX11EffectShaderResourceVariable* sRayleighMap;
	ID3DX11EffectShaderResourceVariable* sMieMap;

	
	class Dome* dome;
	class Moon* moon;

	//class Cloud* cloud;

	class PerlinCloud* perlinCloud;

	//ConstantBuffer* cloudBuffer;
	//ID3DX11EffectConstantBuffer* sCloudBuffer;

	ConstantBuffer* perlincloudBuffer;
	ID3DX11EffectConstantBuffer* sPerlinCloudBuffer;
};