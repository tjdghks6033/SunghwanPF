#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Terrain.fx"

float4 PS(VertexTerrain input) : SV_Target0
{
    float3 diffuse = GetLayerColor(input.Uv);
    float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);

    float4 brushColor = GetBrushColor(input.wPosition);
    float4 lineColor = GetLineColor(input.wPosition);

    return float4(diffuse * NdotL, 1) + brushColor + lineColor;
}

RasterizerState RS
{
    FillMode = WireFrame;
};

technique11 T0
{
    P_VP(P0, VS, PS)
    P_RS_VP(P1, RS, VS, PS)
}