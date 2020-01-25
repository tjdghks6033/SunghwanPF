#include "00_Global.fx"
#include "00_Deffered.fx"
#include "00_Model.fx"

Texture2D DefferedMaps[6];
DepthStencilState Deffered_DepthStencil_State;

struct DefferedDesc
{
    float4 Perspective;
    float2 SpecularPowerRange;
};

cbuffer CB_Deffered
{
    DefferedDesc Deffered;
};

///////////////////////////////////////////////////////////////////////////////

static const float2 ScreenNDC[4] = { float2(-1, +1), float2(+1, +1), float2(-1, -1), float2(+1, -1) };

struct VertexOutput_Deffered
{
    float4 Position : SV_Position;
    float2 Screen : Position1;
};

VertexOutput_Deffered VS_Deffered(uint VertexID : SV_VertexID)
{
    VertexOutput_Deffered output;
    
    output.Position = float4(ScreenNDC[VertexID], 0, 1);
    output.Screen = output.Position.xy;
    
    return output;
}

///////////////////////////////////////////////////////////////////////////////

struct PixelOutput_GBuffer
{
    float4 Diffuse : SV_Target0;
    float4 Specular : SV_Target1;
    float4 Emissive : SV_Target2;
    float4 Normal : SV_Target3;
    float4 Tangent : SV_Target4;
};

///////////////////////////////////////////////////////////////////////////////

PixelOutput_GBuffer PS_PackGBuffer(MeshOutput input)
{
    Texture(Material.Diffuse, DiffuseMap, input.Uv);
    Texture(Material.Specular, SpecularMap, input.Uv);
    
    
    PixelOutput_GBuffer output;

    output.Diffuse = float4(Material.Diffuse.rgb, 1);
    output.Specular = Material.Specular;
    output.Specular.a = max(1e-6f, (Material.Specular.a - Deffered.SpecularPowerRange.x) / Deffered.SpecularPowerRange.y);
    output.Emissive = Material.Emissive;
    output.Normal = float4(input.Normal, 1);
    output.Tangent = float4(input.Tangent, 1);
    
    return output;
}

///////////////////////////////////////////////////////////////////////////////

void UnpackGBuffer(inout float4 position, in float2 screen, out MaterialDesc material, out float3 normal, out float3 tangent)
{  
    material.Ambient = float4(0, 0, 0, 1);
    material.Diffuse = DefferedMaps[1].Load(int3(position.xy, 0));
    material.Specular = DefferedMaps[2].Load(int3(position.xy, 0));
    material.Specular.a = Deffered.SpecularPowerRange.x + Deffered.SpecularPowerRange.y * material.Specular.a;
    material.Emissive = DefferedMaps[3].Load(int3(position.xy, 0));
    
    normal = DefferedMaps[4].Load(int3(position.xy, 0)).rgb;
    tangent = DefferedMaps[5].Load(int3(position.xy, 0)).rgb;
    
    
    float depth = DefferedMaps[0].Load(int3(position.xy, 0)).r;
    float linearDepth = Deffered.Perspective.z / (depth + Deffered.Perspective.w);
    
    position.xy = screen * Deffered.Perspective.xy * linearDepth;
    position.z = linearDepth;
    position.w = 1.0f;
    position = mul(position, ViewInverse);

}

float4 PS_Directional(VertexOutput_Deffered input) : SV_Target
{
    float4 position = input.Position;
    
    float3 normal = 0, tangent = 0;
    MaterialDesc material = MakeMaterial();    
    
    UnpackGBuffer(position, input.Screen, material, normal, tangent);
    
    
    material.Diffuse = float4(1, 1, 1, 1);
    
    MaterialDesc result = MakeMaterial();
    MaterialDesc output = MakeMaterial();
    ComputeLight_Deffered(result, material, normal, position.xyz);
    AddMaterial(output, result);
    
    return float4(MaterialToColor(output), 1.0f);
}

///////////////////////////////////////////////////////////////////////////////

DepthStencilState DepthStencil_Light;
BlendState Blend_Additive2;

technique11 T0
{
    //Deffered - PackGBuffer
    P_DSS_VP(P0, DepthStencil_Light, VS_Mesh, PS_PackGBuffer)
    P_DSS_VP(P1, DepthStencil_Light, VS_Model, PS_PackGBuffer)
    P_DSS_VP(P2, DepthStencil_Light, VS_Animation, PS_PackGBuffer)

    //Deffered - Directional
    P_DSS_VP(P3, DepthStencil_Light, VS_Deffered, PS_Directional)
}