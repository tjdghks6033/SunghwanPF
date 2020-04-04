#pragma once

class Terrain : public Renderer
{
public:
	typedef VertexTextureNormalAlpha TerrainVertex;

public:
	Terrain(Shader* shader, wstring heightFile);
	~Terrain();

	void Update();
	void Render();

	void BaseMap(wstring file);
	void LayerMap(wstring file, wstring alpha);
	void LayerMap2(wstring file, wstring alpha);
	void LayerMap3(wstring file, wstring alpha);

	float GetHeight(Vector3& position);
	float GetHeightPick(Vector3& position);
	Vector3 GetPickedPosition();
	void RaiseHeight(Vector3& position, UINT type, UINT range);
	void DownHeight(Vector3& position, UINT type, UINT range);
	void FlatHeight(Vector3& position, UINT type, UINT range);
	void Smoothing(Vector3& position, UINT type, UINT range);
	void Slope(Vector3& position, UINT type, UINT range);
	void Noise(Vector3& position, UINT type, UINT range);
	void Paint(Vector3& position, UINT type, UINT range);

private:
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalData();

private:
	struct BrushDesc
	{
		Color Color = D3DXCOLOR(0, 1, 0, 1);
		Vector3 Location;
		UINT Type = 2;
		UINT Range = 7;
		float Padding[3];
	} brushDesc;

	struct LineDesc
	{
		Color Color = D3DXCOLOR(1, 1, 1, 1);
		UINT Visible = 0;
		float Thickness = 0.01f;
		float Size = 5.0f;
		float Padding;
	} lineDesc;

private:
	Texture* heightMap;

	UINT width;
	UINT height;

	TerrainVertex* vertices;
	UINT* indices;

	Texture* baseMap = NULL;
	ID3DX11EffectShaderResourceVariable* sBaseMap;

	Texture* alphaMap = NULL;
	ID3DX11EffectShaderResourceVariable* sAlphaMap;

	Texture* layerMap = NULL;
	Texture* layerMap2 = NULL;
	Texture* layerMap3 = NULL;
	ID3DX11EffectShaderResourceVariable* sLayerMap;
	ID3DX11EffectShaderResourceVariable* sLayerMap2;
	ID3DX11EffectShaderResourceVariable* sLayerMap3;

	ConstantBuffer* brushBuffer;
	ID3DX11EffectConstantBuffer* sBrushBuffer;

	ConstantBuffer* lineBuffer;
	ID3DX11EffectConstantBuffer* sLineBuffer;


	bool is_mapeditor = false;

	bool is_flattening = false;
	bool is_clicked = false;
	bool is_smoothing = false;
	bool is_slope = false;
	bool is_noise = false;
	bool is_raise = false;
	bool is_paint = false;
	float noise_var = 1.0f;
	int noise_freq = 1;
	Vector3 prevlocation = Vector3(0.0f, 0.0f, 0.0f);
	int layernum = 1;
	bool is_hovered = false;
};
