#pragma once

class Cloud : public Renderer
{
public:
	Cloud(Shader* shader);
	~Cloud();

	void Update();
	void Render();
	void PostRender();

private:
	void CreateTexture();

private:
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;
	ID3DX11EffectShaderResourceVariable* sSRV;

	Vector3 scale = Vector3(0, 0, 0);
	Vector3 rotation = Vector3(0, 0, 0);
	Vector3 position2 = Vector3(0, 0, 0);

	Render2D* render2D;
};