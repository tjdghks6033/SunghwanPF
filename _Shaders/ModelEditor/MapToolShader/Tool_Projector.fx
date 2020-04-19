#include "../000_Header.fx"
#include "../000_Light.fx"
#include "../000_Terrain.fx"

TextureCube CubeMap;

float4 PS(MainOutput input) : SV_TARGET0
{
    Texture(Material.Diffuse, DiffuseMap, input.Uv);

    NormalMapping(input.Uv, input.Normal, input.Tangent);
    
    Texture(Material.Specular, SpecularMap, input.Uv);
    
    float4 color = 0;
    ComputeLight(color, input.Normal, input.wPosition);
    ComputePointLights(color, input.wPosition);
    ComputeSpotLights(color, input.wPosition);
    return color;
}

float4 PS_Cube(MainOutput input) : SV_TARGET0
{
    Texture(Material.Diffuse, DiffuseMap, input.Uv);

    NormalMapping(input.Uv, input.Normal, input.Tangent);
    
    Texture(Material.Specular, SpecularMap, input.Uv);
    
    float4 color = 0;
    ComputeLight(color, input.Normal, input.wPosition);
    ComputePointLights(color, input.wPosition);
    ComputeSpotLights(color, input.wPosition);

    float3 eye = normalize(input.wPosition - ViewPosition());
    float3 r = reflect(eye, normalize(input.Normal));

    // 스넬 법칙 , 플레넬의 방정식
    //
    color *= (0.85f + CubeMap.Sample(LinearSampler, r) * 0.75f);

    return color;

    //return CubeMap.Sample(LinearSampler, r);
    //return color + CubeMap.Sample(LinearSampler, r);

}

//-----------------------------------------------------------------------------
// Techniques
//-----------------------------------------------------------------------------

technique11 T0
{


    P_VP(PO, VS_Main,PS)
    P_VP(P1, VS_Model, PS)
    

    P_VP(P2, VS_Main, PS_Cube)
    P_VP(P3, VS_Model, PS_Cube)
}