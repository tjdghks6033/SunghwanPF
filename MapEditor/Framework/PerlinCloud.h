#pragma once

class PerlinCloud : public Renderer
{
public:
	PerlinCloud(Shader* shader);
	~PerlinCloud();

	void Update();
	void Render();
	void PostRender();

private:
	struct SkyPlaneType
	{
		float x, y, z;
		float tu, tv;
	};

	struct VertexType
	{
		Vector3 position;
		Vector2 texture;
	};

	int GetIndexCount() { return indexCount; }
	ID3D11ShaderResourceView* GetCloudTexture() { return CloudTexture->SRV(); }
	ID3D11ShaderResourceView* GetPerturbTexture() { return PerturbTexture->SRV(); }

	float GetScale() { return m_scale; }
	float GetBrightness() { return m_brightness; }
	float GetTranslation() { return m_translation; }

private:
	SkyPlaneType* skyPlane = nullptr;
	Texture * CloudTexture = nullptr;
	Texture * PerturbTexture = nullptr;
	float m_scale = 0.0f;
	float m_brightness = 0.0f;
	float m_translation = 0.0f;

private:
	//ID3D11Texture2D* texture;
	//ID3D11ShaderResourceView* srv;
	ID3DX11EffectShaderResourceVariable* sSRVCloud;
	ID3DX11EffectShaderResourceVariable* sSRVPertube;

	Vector3 scale = Vector3(0, 0, 0);
	Vector3 rotation = Vector3(0, 0, 0);
	Vector3 position2 = Vector3(0, 0, 0);

	Render2D* render2D;
};