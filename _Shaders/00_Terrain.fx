Texture2D BaseMap;
Texture2D AlphaMap;
Texture2D LayerMap;
Texture2D LayerMap2;
Texture2D LayerMap3;

struct VertexTerrain
{
	float4 Position : SV_Position0;
	float3 wPosition : Position1;
	float2 Uv : Uv0;
	float3 Normal : Normal0;
	float4 Color : Color0;
};


VertexTerrain VS_Terrain(VertexTextureNormalAlpha input)
{
	VertexTerrain output;
	output.Position = mul(input.Position, World);
	output.wPosition = output.Position.xyz;
    
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Normal = mul(input.Normal, (float3x3) World);
	output.Uv = input.Uv;

	output.Color = input.Color;

	return output;
}

///////////////////////////////////////////////////////////////////////////////

struct BrushDesc
{
	float4 Color;
	float3 Location;
	uint Type;
	uint Range;
};

cbuffer CB_Brush
{
	BrushDesc TerrainBrush;
};

float4 GetBrushColor(float3 wPosition)
{
    [flatten]
	if (TerrainBrush.Type == 0)
		return float4(0, 0, 0, 0);

    [flatten]
	if (TerrainBrush.Type == 1)
	{
        [flatten]
		if ((wPosition.x >= (TerrainBrush.Location.x - TerrainBrush.Range)) &&
            (wPosition.x <= (TerrainBrush.Location.x + TerrainBrush.Range)) &&
            (wPosition.z >= (TerrainBrush.Location.z - TerrainBrush.Range)) &&
            (wPosition.z <= (TerrainBrush.Location.z + TerrainBrush.Range)))
		{
			return TerrainBrush.Color;
		}
	}

    [flatten]
	if (TerrainBrush.Type == 2)
	{
		float dx = wPosition.x - TerrainBrush.Location.x;
		float dz = wPosition.z - TerrainBrush.Location.z;
		float dist = sqrt(dx * dx + dz * dz);

        [flatten]
		if (dist <= (float) TerrainBrush.Range)
			return TerrainBrush.Color;
	}
	return float4(0, 0, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////

struct LineDesc
{
	float4 Color;
	uint Visible;
	float Thickness;
	float Size;
};

cbuffer CB_TerrainLine
{
	LineDesc TerrainLine;
};

float4 GetBasicLineColor(float3 wPosition)
{
    [flatten]
	if (TerrainLine.Visible < 1)
		return float4(0, 0, 0, 0);

	float2 grid = wPosition.xz / TerrainLine.Size;
	grid = frac(grid);

	float thick = TerrainLine.Thickness / TerrainLine.Size;

    [flatten]
	if (grid.x < thick || grid.y < thick)
		return TerrainLine.Color;

	return float4(0, 0, 0, 0);
}

float4 GetLineColor(float3 wPosition)
{
    [flatten]
	if (TerrainLine.Visible < 1)
		return float4(0, 0, 0, 0);

	float2 grid = wPosition.xz / TerrainLine.Size;
	float2 range = abs(frac(grid - 0.5f) - 0.5f);

	float2 speed = fwidth(grid);
	float2 pixel = range / speed;

    //return float4(speed, 0, 1);
    //return float4(speed.x, 0, 0, 1);

	float thick = saturate(min(pixel.x, pixel.y) - TerrainLine.Thickness);
	return lerp(TerrainLine.Color, float4(0, 0, 0, 0), thick);
}

///////////////////////////////////////////////////////////////////////////////

float4 GetBaseColor(float2 uv)
{
	return BaseMap.Sample(LinearSampler, uv);
}

float4 GetLayerColor(float2 uv)
{
	float4 base = GetBaseColor(uv);
	float alpha = AlphaMap.Sample(LinearSampler, uv).r;
	float4 layer = LayerMap.Sample(LinearSampler, uv);

	return lerp(base, layer, alpha);

}

float4 GetLayerColor2(float2 uv, float4 color)
{
	float4 base = GetBaseColor(uv);
	float4 layer = LayerMap.Sample(LinearSampler, uv);
	float4 layer2 = LayerMap2.Sample(LinearSampler, uv);
	float4 layer3 = LayerMap3.Sample(LinearSampler, uv);

	base = lerp(base, layer, color.r);
	base = lerp(base, layer2, color.g);
	base = lerp(base, layer3, color.b);

	return base;
}