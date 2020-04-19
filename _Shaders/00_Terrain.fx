Texture2D BaseMap;
Texture2D AlphaMap;
Texture2D LayerMap;
Texture2D LayerMap2;
Texture2D LayerMap3;
Texture2D HeightMap;

struct VertexTerrain
{
	float4 Position : SV_Position0;
	float3 wPosition : Position1;
	float4 sPosition : Position2;
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
	

	output.sPosition = WorldPosition(input.Position);
	output.sPosition = mul(output.sPosition, ShadowView);
	output.sPosition = mul(output.sPosition, ShadowProjection);
	
	output.Normal = mul(input.Normal, (float3x3) World);
	output.Uv = input.Uv;

	output.Color = input.Color;

	return output;
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

float3 GetLayerColor2(float2 uv, float4 color)
{
	float4 base = GetBaseColor(uv);
	float4 layer = LayerMap.Sample(LinearSampler, uv);
	float4 layer2 = LayerMap2.Sample(LinearSampler, uv);
	float4 layer3 = LayerMap3.Sample(LinearSampler, uv);

	base = lerp(base, layer, color.r);
	base = lerp(base, layer2, color.g);
	base = lerp(base, layer3, color.b);

	return base.rgb;
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


float4 PS_Terrain(VertexTerrain input) : SV_Target0
{
	float3 diffuse = GetLayerColor2(input.Uv, input.Color);
	float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);

	float4 brushColor = GetBrushColor(input.wPosition);
	float4 lineColor = GetLineColor(input.wPosition);
    
	//return float4(diffuse * NdotL, 1) + brushColor + lineColor;	
	float4 color = float4(diffuse * NdotL, 1) + brushColor + lineColor;
	
	
	return PS_Shadow(input.sPosition, color);
}


///////////////////////////////////////////////////////////////////////////////
// TerrainLOD
///////////////////////////////////////////////////////////////////////////////
cbuffer CB_TerrainLod
{
	float MinDistance;
	float MaxDistance;
	float MinTessellation;
	float MaxTessellation;

	float TexelCellSpaceU;
	float TexelCellSpaceV;
	float WorldCellSpace;
	float TerrainHeightRatio;

	float2 TextureScale;
	float CB_Terrain_Padding2[2];

	float4 WorldFrustumPlanes[6];
};

struct VertexInput_TerrainLod
{
	float4 Position : Position;
	float2 Uv : Uv;
	float2 BoundY : BoundY; //�ִ��ּҳ���
};

struct VertexOutput_TerrainLod
{
	float4 Position : Position;
	float2 Uv : Uv;
	float2 BoundY : BoundY;
};

VertexOutput_TerrainLod VS_TerrainLod(VertexInput_TerrainLod input)
{
	VertexOutput_TerrainLod output;

	output.Position = input.Position;
	output.Uv = input.Uv;
	output.BoundY = input.BoundY;

	return output;
}

struct ConstantHullOutput_TerrainLod
{
	float Edge[4] : SV_TessFactor;
	float Inside[2] : SV_InsideTessFactor;
};

bool AAbbBehindPlaneTest(float3 center, float3 extents, float4 plane)
{
	float3 n = abs(plane.xyz);
	float r = dot(extents, n);
	float s = dot(float4(center, 1), plane);
	
	return (s + r) < 0.0f;
}

bool InFrustumCube(float3 center, float3 extents)
{
	[unroll(6)]
	for (int i = 0; i < 6; i++)
	{
		[flatten]
		if (AAbbBehindPlaneTest(center, extents, WorldFrustumPlanes[i]))		
			return true;
	}
	
	return false;
}

float TessellationFactor(float3 position)
{
	position = float3(position.x, 0.0f, position.z);
	float3 view = float3(ViewPosition().x, 0.0f, ViewPosition().z);

	float d = distance(position, view);
	float s = saturate((d - MaxDistance) / (MinDistance - MaxDistance));
	
	//return s * 63;
	return lerp(MinTessellation, MaxTessellation, s);
}

ConstantHullOutput_TerrainLod CHS_TerrainLod(InputPatch<VertexOutput_TerrainLod, 4> input)
{
	float minY = input[0].BoundY.x;
	float maxY = input[0].BoundY.y;
	
	float3 vMin = float3(input[2].Position.x, minY, input[2].Position.z);
	float3 vMax = float3(input[1].Position.x, maxY, input[1].Position.z);
	
	float3 boxCenter = (vMin + vMax) * 0.5f;
	float3 boxExtents = (vMax - vMin) * 0.5f;

	ConstantHullOutput_TerrainLod output;
	
	[flatten]
	if (InFrustumCube(boxCenter, boxExtents))
	{
		output.Edge[0] = output.Edge[1] = output.Edge[2] = output.Edge[3] = -1;
		output.Inside[0] = output.Inside[1] = -1;

		return output;
	}
	
	float3 e0 = (input[0].Position + input[2].Position).xyz * 0.5f;
	float3 e1 = (input[0].Position + input[1].Position).xyz * 0.5f;
	float3 e2 = (input[1].Position + input[3].Position).xyz * 0.5f;
	float3 e3 = (input[2].Position + input[3].Position).xyz * 0.5f;

	output.Edge[0] = TessellationFactor(e0);
	output.Edge[1] = TessellationFactor(e1);
	output.Edge[2] = TessellationFactor(e2);
	output.Edge[3] = TessellationFactor(e3);

    
	float3 c = (input[0].Position.xyz + input[1].Position.xyz + input[2].Position.xyz + input[3].Position.xyz) * 0.25f;
	output.Inside[0] = TessellationFactor(c);
	output.Inside[1] = TessellationFactor(c);
	
	

	return output;
}

struct HullOutput_TerrainLod
{
	float4 Position : Position0;
	float2 Uv : Uv0;
};

[domain("quad")]
//[partitioning("fractional_even")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CHS_TerrainLod")]
HullOutput_TerrainLod HS_TerrainLod(InputPatch<VertexOutput_TerrainLod, 4> input, uint id : SV_OutputControlPointID)
{
	HullOutput_TerrainLod output;
	output.Position = input[id].Position;
	output.Uv = input[id].Uv;

	return output;
}

struct DomainOutput_TerrainLod
{
	float4 Position : SV_Position0;
	float3 wPosition : Position1;
	float2 Uv : Uv0;
	float2 TiledUv : Uv1;
};

[domain("quad")]
DomainOutput_TerrainLod DS_TerrainLod
(
    ConstantHullOutput_TerrainLod input, float2 uv : SV_DomainLocation, OutputPatch<HullOutput_TerrainLod, 4> patch //, uint patchID : SV_PrimitiveID
)
{
	DomainOutput_TerrainLod output;
	float3 p0 = lerp(patch[0].Position, patch[1].Position, uv.x).xyz;
	float3 p1 = lerp(patch[2].Position, patch[3].Position, uv.x).xyz;
	float3 position = lerp(p0, p1, uv.y);
	
	float2 uv0 = lerp(patch[0].Uv, patch[1].Uv, uv.x);
	float2 uv1 = lerp(patch[2].Uv, patch[3].Uv, uv.x);
	output.Uv = lerp(uv0, uv1, uv.y);
	
	position.y = HeightMap.SampleLevel(LinearSampler, output.Uv, 0).r * 255.0f / TerrainHeightRatio;
	
	output.Position = WorldPosition(float4(position, 1));
	output.wPosition = output.Position.xyz;
	
	output.Position = ViewProjection(output.Position);
	output.TiledUv = output.Uv * TextureScale;
	
	return output;
}

float4 PS_TerrainLod(DomainOutput_TerrainLod input) : SV_Target
{
	float2 left = input.Uv + float2(-TexelCellSpaceU, 0.0f);
	float2 right = input.Uv + float2(+TexelCellSpaceU, 0.0f);

	float2 top = input.Uv + float2(0.0f, -TexelCellSpaceV);
	float2 bottom = input.Uv + float2(0.0f, TexelCellSpaceV);
	
	float leftY = HeightMap.SampleLevel(LinearSampler, left, 0).r * 255.0f / TerrainHeightRatio;
	float rightY = HeightMap.SampleLevel(LinearSampler, right, 0).r * 255 / TerrainHeightRatio;
	float topY = HeightMap.SampleLevel(LinearSampler, top, 0).r * 255 / TerrainHeightRatio;
	float bottomY = HeightMap.SampleLevel(LinearSampler, bottom, 0).r * 255 / TerrainHeightRatio;
	
	float3 tangent = normalize(float3(WorldCellSpace * 2.0f, rightY - leftY, 0.0f));
	float3 biTangent = normalize(float3(0.0f, bottomY - topY, WorldCellSpace * -2.0f));
	float3 normal = normalize(cross(tangent, biTangent));

	Material.Diffuse = GetLayerColor(input.Uv);
	NormalMapping(input.Uv, normal, tangent);
	
	return Material.Diffuse;
}