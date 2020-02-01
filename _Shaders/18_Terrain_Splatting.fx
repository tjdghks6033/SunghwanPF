#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Terrain.fx"

float4 PS_Terrain(VertexTerrain input) : SV_Target0
{
	float3 diffuse = GetLayerColor2(input.Uv, input.Color); //LayerMap.Sample(LinearSampler, input.Uv).rgb;
	float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);

	float4 brushColor = GetBrushColor(input.wPosition);
	float4 lineColor = GetLineColor(input.wPosition);
    
   //float3 diffuse2 = LayerMap2.Sample(LinearSampler, input.Uv).rgb;
   //float3 diffuse3 = LayerMap3.Sample(LinearSampler, input.Uv).rgb;

    //float alpha = GetLayerColor2(input.Uv, input.Color);
    
    //if (input.Color.r > 0.0f)
    //    return float4(diffuse * NdotL, 1) + brushColor + lineColor + alpha; //+ input.Color;
    //else if (input.Color.g > 0.0f)
    //    return float4(diffuse2 * NdotL, 1) + brushColor + lineColor + alpha; //+ input.Color;
    //else if (input.Color.b > 0.0f)
    //    return float4(diffuse3 * NdotL, 1) + brushColor + lineColor + alpha; //+ input.Color;
    //else
    //    return float4(diffuse * NdotL, 1) + brushColor + lineColor + alpha; //+ input.Color;
    
	return float4(diffuse * NdotL, 1) + brushColor + lineColor;
}

RasterizerState RS
{
    FillMode = WireFrame;
};

technique11 T0
{
    P_VP(P0, VS_Terrain, PS_Terrain)
    P_RS_VP(P1, RS, VS_Terrain, PS_Terrain)

	P_VTP(P2, VS_TerrainLod, HS_TerrainLod, DS_TerrainLod, PS_TerrainLod)
	P_RS_VTP(P3, FillMode_WireFrame, VS_TerrainLod, HS_TerrainLod, DS_TerrainLod, PS_TerrainLod)
}