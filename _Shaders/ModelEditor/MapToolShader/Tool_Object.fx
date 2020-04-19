//#include "../000_Header.fx"
//#include "../000_Light.fx"
//#include "../000_Model.fx"

#include "Tool_Header.fx"
#include "Tool_Light.fx"
#include "Tool_Model.fx"

///////////////////////////////////////////////////////////////////////////////


struct VertexInput
{
   float4 Position : POSITION0;
   float2 Uv : Uv0;
   float3 Normal : Normal0;
   float3 Tangent : Tangent0;
  // matrix Transform : InstTransform0;
   uint InstID : SV_InstanceID0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION0;
    float3 wPosition : POSITION1;
    float2 Uv : Uv0;
    float3 Normal : Normal0;
    float3 Tangent : Tangent0;
    uint InstID : ID0;
};


VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    
    //output.Position = mul(input.Position, input.Transform); //
    output.Position = WorldPosition(input.Position);
    output.wPosition = output.Position.xyz;

    output.Position = ViewProjection(output.Position);
    output.Normal = WorldNormal(input.Normal);
    output.Tangent = WorldTangent(input.Tangent);

    output.InstID = input.InstID;
    output.Uv = input.Uv;

    return output;
    
}
///////////////////////////////////////////////////////////////////////////////

float4 PS(VertexOutput input) : SV_TARGET0
{
   // float3 uvw = float3(input.Uv, TextureType[input.InstID]);

    
    //Textures(Material.Diffuse, SpecularMaps, uvw);
    //InstanceNormalMapping(uvw, input.Normal, input.Tangent);
    //Textures(Material.Specular, SpecularMaps, uvw);
    return Material.Diffuse;
    return float4(1, 1, 1, 1);
    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);

    Texture(Material.Diffuse, DiffuseMap, input.Uv);
    NormalMapping(input.Uv, input.Normal, input.Tangent);
    Texture(Material.Specular, SpecularMap, input.Uv);

    
    float4 color = 0;
    ComputeLight(color, input.Normal, input.wPosition);
    ComputePointLights(color, input.wPosition);
    ComputeSpotLights(color, input.wPosition);
    //return Colors[input.InstID] * color;
}

///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Techniques
//-----------------------------------------------------------------------------

technique11 T0
{
    //Render
    P_VP(PO, VS, PS)
   //P_VP(P1, VS_Model, PS)
}