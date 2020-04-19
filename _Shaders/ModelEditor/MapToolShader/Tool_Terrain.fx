#include "../000_Header.fx"
#include "../000_Light.fx"
#include "../000_Terrain.fx"

cbuffer CB_Projector
{
    matrix ProjectorView;
    matrix ProjectorProjection;

    float4 ProjectorColor;

};


struct VertexOutput
{
    float4 Position : SV_Position0;
    float3 oPosition : Position1;
    float3 wPosition : Position2;
    float4 wvpPosition : Position3;

    float3 Normal : Normal0;
    float4 Color : Color0;
    float2 Uv : UV0;
};



VertexOutput VS(VertexColorTextureNormal input)
{
    VertexOutput output;

    output.Position = WorldPosition(input.Position);
    output.wPosition = output.Position.xyz;
    output.wvpPosition  = output.Position;

    output.Position = ViewProjection(output.Position);
    output.Normal = WorldNormal(input.Normal);
    output.Color = input.Color;

    output.oPosition = input.Position.xyz;
    output.Uv = input.Uv;

    return output;
}


void ProjectorPosition(inout float4 wvp, float4 position)
{
    wvp = WorldPosition(position);
    wvp = mul(wvp, ProjectorView);
    wvp = mul(wvp, ProjectorProjection);
}


VertexOutput VS_Terrain(VertexColorTextureNormal input)
{
    VertexOutput output = VS(input);

    ProjectorPosition(output.wvpPosition, input.Position);

    return output;
}

///////////////////////////////////////////////////////////////////////////////

float4 PS(VertexOutput input) : SV_TARGET0
{
    
    float4 color = GetTerrainColors(input.Uv, input.Color);

    float3 normal = normalize(input.Normal);
    float3 light = -GlobalLight.Direction;
    float NdotL = dot(normal, light);

    float3 colorLine = GetGridLineColor(input.oPosition);
    float3 range = GetBrushColor(input.oPosition);

    //

    //
    float4 brushTexture = GetBrushTexture(input.oPosition, float4(0, 0, 0, 0));
    
    float4 color2 = (color * NdotL);
    
    float4 light2 = 0;

    ComputePointLights(color2, input.oPosition);
    ComputeSpotLights(color2, input.oPosition);

    

    // -1 ~ 1 까지 떨어진 NDC 공간 
    //float2 uv = 0;
    //uv.x = input.wvpPosition.x / input.wvpPosition.w * 0.5f + 0.5f;
    //uv.y = -input.wvpPosition.y / input.wvpPosition.w * 0.5f + 0.5f;
    
    //[flatten]
    //if(saturate(uv.x) == uv.x && saturate(uv.y) == uv.y)
    //{
    //    float4 map = ProjectorMap.Sample(LinearSampler, uv);

    //    map *= ProjectorColor;

    //    color = lerp(color, map, map.a);
    //}


    return color2 + float4(colorLine, 1) + float4(range, 1) + brushTexture;
}

///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Techniques
//-----------------------------------------------------------------------------

RasterizerState RS
{
    FillMode = Wireframe;
};


technique11 T0
{
    //Render
    P_VP(P0, VS_Terrain, PS)

    //WireFrames;
    P_RS_VP(P1,RS, VS_Terrain, PS)
}