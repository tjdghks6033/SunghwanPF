#pragma once

class Trail : public Renderer
{
public:
	struct InitializeDesc
	{
		Matrix trail;
	} desc;

public:
	Trail(InitializeDesc & initDesc, UINT count );
	~Trail();

	void Update(Matrix mat);
	void Render();

	UINT& Pass() { return pass; }
	void Pass(UINT val) { pass = val; }

	void UpdateTransform(Matrix mat);

	void SetMatrix(Matrix mat) { this->sword_mat = mat; }
	Matrix& GetMatrix() { return sword_mat; }

private:
	void GenerateTrail();

	void UpdateTrailBuffer();

private:
	Texture* texture;

	Matrix sword_mat;

	VertexTexture* vertices;

	UINT pass = 0;
	UINT count = 0;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
	ID3DX11EffectShaderResourceVariable* sTrailMap;

	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
	Vector3 translation = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);
};