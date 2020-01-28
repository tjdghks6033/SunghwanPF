#pragma once

class GBuffer
{
public:
	GBuffer(Shader* shader, UINT width = 0, UINT height = 0);
	~GBuffer();

	void PackGBuffer();
	void Render();
	void DebugRender();

	void SetDebug(bool val) { bDebug = val; }
	void SetDrawPointLights(bool val) { bDrawPointLights = val; }
	void SetDrawSpotLights(bool val) { bDrawSpotLights = val; }

private:
	void CreateDepthStencilView();
	void CreateDepthStencilState();
	void CreateRasterizerState();

private:
	void RenderDirectionalLight();

	void CalcPointLights(UINT count);
	void RenderPointLights();

	void CalcSpotLights(UINT count);
	void RenderSpotLights();

private:
	struct PointLightDesc
	{
		float TessFactor = 32.0f;
		float Padding[3];

		Matrix Projection[MAX_POINT_LIGHT];
		PointLight PointLight[MAX_POINT_LIGHT];
	} pointLightDesc;

	struct SpotLightDesc
	{
		float TessFactor = 32.0f;
		float Padding[3];

		Vector4 Angle[MAX_SPOT_LIGHT];
		Matrix Projection[MAX_SPOT_LIGHT];

		SpotLight SpotLight[MAX_SPOT_LIGHT];
	} spotLightDesc;

private:
	bool bDebug = false;
	bool bDrawPointLights = true;
	bool bDrawSpotLights = true;


	Shader* shader;
	UINT width, height;

	RenderTarget* diffuseRTV;
	RenderTarget* specularRTV;
	RenderTarget* emissiveRTV;
	RenderTarget* normalRTV;
	RenderTarget* tangentRTV;
	DepthStencil* depthStencil;
	Viewport* viewport;

	ID3D11DepthStencilView* depthStencilReadOnly;
	ID3D11DepthStencilState* packDSS;
	ID3D11DepthStencilState* noDepthWriteLessDSS;
	ID3D11DepthStencilState* noDepthWriteGreatorDSS;
	ID3DX11EffectDepthStencilVariable* sDSS;

	ID3D11RasterizerState* debugRSS;
	ID3D11RasterizerState* lightRSS;
	ID3DX11EffectRasterizerVariable* sRSS;

	ID3DX11EffectShaderResourceVariable* sSrvs;


	ConstantBuffer* pointLightBuffer;
	ID3DX11EffectConstantBuffer* sPointLightBuffer;

	ConstantBuffer* spotLightBuffer;
	ID3DX11EffectConstantBuffer* sSpotLightBuffer;

	Render2D* debug2D[6];
};