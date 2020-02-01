#pragma once

class Trail
{
public:
	Trail(Matrix* transform, UINT count);
	~Trail();

	void Update();
	void Render();

	UINT& Pass() { return pass; }
	void Pass(UINT val) { pass = val; }

	Matrix& GetMatrix() { return worlds[0]; }

private:
	void GenerateTrail();

	void UpdateTrailBuffer();

	void UpdateTransform();	

private:
	struct Desc
	{
		Matrix trail;
	} desc;

private:
	Shader* shader;
	Texture* texture;

	Matrix* world;

	VertexTexture* vertices;

	UINT pass = 0;
	UINT count = 0;

	PerFrame* perFrame = NULL;

	ID3D11Buffer* vertexBuffer;

	vector<Transform *> transforms;
	Matrix worlds[MAX_MODEL_TRAIL];

	VertexBuffer* instanceBuffer;


	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Viewport* viewport;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
	//ID3DX11EffectShaderResourceVariable* sTrailMap;
	ID3DX11EffectShaderResourceVariable* sTrailMap;

	ID3D11SamplerState* samplerState;
	ID3DX11EffectSamplerVariable* sSamplerState;
};