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

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
	//ID3DX11EffectShaderResourceVariable* sTrailMap;

};