#pragma once

class Billboard : public Renderer
{
public:
	Billboard(Shader* shader);
	~Billboard();

	void Add(Vector3& position, Vector2& scale, float & random);
	void AddTexture(wstring file);


	void Update();
	void Render();

private:
	struct VertexScale
	{
		Vector3 Position;
		Vector2 Scale;
		float Random;
	};

private:
	vector<VertexScale> vertices;

	bool is_right;

	vector<wstring> textureFiles;
	TextureArray* textures = NULL;
	ID3DX11EffectShaderResourceVariable* sMaps;
};