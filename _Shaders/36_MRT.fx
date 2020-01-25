#include "00_Global.fx"
#include "00_Light.fx"

cbuffer CB_Render2D
{
    matrix View2D;
    matrix Projection2D;
};

struct VertexOutput
{
    float4 Position : SV_Position0;
    float2 Uv : Uv0;
};

VertexOutput VS(VertexTexture input)
{
    VertexOutput output;

    output.Position = WorldPosition(input.Position);
    output.Position = mul(output.Position, View2D);
    output.Position = mul(output.Position, Projection2D);
    output.Uv = input.Uv;

    return output;
}

struct PixelOutput
{
    float4 Target0 : SV_Target0;
    float4 Target1 : SV_Target1;
};

PixelOutput PS(VertexOutput input)
{
    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
    
    PixelOutput output;
    output.Target0 = 1 - diffuse;
    
    float3 color = diffuse.rgb;
    float grayscale = (color.r + color.g + color.b) / 3.0f;

    output.Target1 = float4(grayscale, grayscale, grayscale, 1.0f);

    return output;
}

technique11 T0
{
    P_DSS_VP(P0, DepthEnable_False, VS, PS)
}