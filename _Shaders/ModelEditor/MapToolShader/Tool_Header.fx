cbuffer CB_PerFrame
{
    matrix View;
    matrix ViewInverse;
    matrix Projection;
    matrix VP;

    float Time;
};

cbuffer CB_World
{
    matrix World;
};

Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;

Texture2DArray DiffuseMaps;
Texture2DArray SpecularMaps;
Texture2DArray NormalMaps;


Texture2D ShadowMap;

TextureCube SkyCubeMap;

SamplerState Sampler;
SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
    AddressW = Wrap;
};

SamplerComparisonState ShadowSampler;
///////////////////////////////////////////////////////////////////////////////

int TextureType[2000];
float4 Colors[2000];
Texture2DArray Maps;

///////////////////////////////////////////////////////////////////////////////

struct Vertex
{
    float4 Position : POSITION0;
};

struct VertexNormal
{
    float4 Position : POSITION0;
    float3 Normal : COLOR0;
};

struct VertexColor
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
};

struct VertexColorNormal
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
    float3 Normal : NORMAL0;
};

struct VertexTexture
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
};

struct VertexTextureColor
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
    float4 Color : COLOR0;
};

struct VertexTextureColorNormal
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
    float4 Color : COLOR0;
    float3 Normal : NORMAL0;
};

struct VertexTextureNormal
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
    float3 Normal : NORMAL0;
};

struct VertexColorTextureNormal
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
    float2 Uv : TEXCOORD0;
    float3 Normal : NORMAL0;
};

struct VertexTextureNormalBlend
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
    float3 Normal : NORMAL0;
    float4 BlendIndices : BLENDINDICES0;
    float4 BlendWeights : BLENDWEIGHTS0;
};

struct VertexTextureNormalTangent
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
};

struct VertexTextureNormalTangentBlend
{
    float4 Position : POSITION0;
    float2 Uv : TEXCOORD0;
    float3 Normal : NORMAL0;
    float4 BlendIndices : BLENDINDICES0;
    float4 BlendWeights : BLENDWEIGHTS0;
};


struct MeshVertexInput
{
    float4 Position : POSITION0;
    //float3 wPosition : POSITION1;
    float2 Uv : Uv0;
    float3 Normal : Normal0;
    float3 Tangent : Tangent0;
    matrix Transform : InstTransform0;
    uint InstID : SV_InstanceID0;
};

struct MeshVertexOutput
{
    float4 Position : SV_POSITION0;
    float3 wPosition : POSITION1;
    float2 Uv : Uv0;
    float3 Normal : Normal0;
    float3 Tangent : Tangent0;
    uint InstID : ID0;
};


///////////////////////////////////////////////////////////////////////////////

float4 WorldPosition(float4 position)
{
    return mul(position, World);
}

float4 ViewProjection(float4 position)
{
    return mul(position, VP);
}

float3 WorldNormal(float3 normal)
{
    return mul(normal, (float3x3) World);
}

float3 WorldTangent(float3 tangent)
{
    return mul(tangent, (float3x3) World);
}

float3 ViewPosition()
{
    return ViewInverse._41_42_43;
}

void Texture(inout float4 color, Texture2D t, float2 uv, SamplerState samp)
{
    float4 temp = t.Sample(samp, uv);

    [flatten]
    if (any(temp) == true)
        color = color * temp;
}

void Texture(inout float4 color, Texture2D t, float2 uv)
{
    Texture(color, t, uv, LinearSampler);
}

void TextureArray(inout float4 color, float4 inst)
{
    [flatten] 
    if (any(inst) == true)
    {
        color = color * inst;
    }
}

void Textures(inout float4 color, Texture2DArray t, float3 uvw, SamplerState samp)
{
    float4 temp = t.Sample(samp, uvw);

    [flatten]
    if (any(temp) == true)
        color = color * temp;
}

void Textures(inout float4 color, Texture2DArray t, float3 uvw)
{
    Textures(color, t, uvw, LinearSampler);
}





///////////////////////////////////////////////////////////////////////////////

struct VertexMesh
{
    float4 Position : Position0;
    float2 Uv : Uv0;
    float3 Normal : Normal0;
    float3 Tangent : Tangent0;
};

cbuffer CB_Shadow
{
    matrix ShadowView;
    matrix ShadowProjection;

    float2 ShadowMapSize;
    float ShadowBias;

    uint ShadowIndex;
};

struct DepthOutput
{
    float4 Position : SV_Position0;
    float4 sPosition : Position1;
};

DepthOutput VS_Main_Depth(VertexMesh input)
{
    DepthOutput output;
    output.Position = WorldPosition(input.Position);
    output.Position = mul(output.Position, ShadowView);
    output.Position = mul(output.Position, ShadowProjection);

    output.sPosition = output.Position;

    return output;
}

float4 PS_Depth(DepthOutput input) : SV_Target0
{
    float depth = input.sPosition.z / input.sPosition.w;

    return float4(0, 0, depth, 1);
}

///////////////////////////////////////////////////////////////////////////////

struct MainOutput
{
    float4 Position : SV_Position0;
    float4 wvpPosition : Position1;
    float3 oPosition : Position2;
    float3 wPosition : Position3;
    float4 sPosition : Position4;

    float2 Uv : Uv0;
    float3 Normal : Normal0;
    float3 Tangent : Tangent0;
};

MainOutput VS_Main(VertexMesh input)
{
    MainOutput output;

    output.oPosition = input.Position.xyz;
    output.Position = WorldPosition(input.Position);
    output.wPosition = output.Position.xyz;

    output.Position = ViewProjection(output.Position);
    output.wvpPosition = output.Position;

    output.Normal = WorldNormal(input.Normal);
    output.Tangent = WorldTangent(input.Tangent);
    output.Uv = input.Uv;

    output.sPosition = WorldPosition(input.Position);
    output.sPosition = mul(output.sPosition, ShadowView);
    output.sPosition = mul(output.sPosition, ShadowProjection);

    return output;
}

///////////////////////////////////////////////////////////////////////////////

struct MainOutput_GS
{
    float4 Position : Position0;
    float3 oPosition : Position2;
    float3 wPosition : Position3;

    float2 Uv : Uv0;
    float3 Normal : Normal0;
    float3 Tangent : Tangent0;
};

MainOutput_GS VS_Main_GS(VertexMesh input)
{
    MainOutput_GS output;

    output.oPosition = input.Position.xyz;
    output.Position = WorldPosition(input.Position);
    output.wPosition = output.Position.xyz;

    output.Normal = WorldNormal(input.Normal);
    output.Tangent = WorldTangent(input.Tangent);
    output.Uv = input.Uv;

    return output;
}

///////////////////////////////////////////////////////////////////////////////

#define P_V(name, vs) \
pass name \
{ \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(NULL); \
}

#define P_VP(name, vs, ps) \
pass name \
{ \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_VP(name, rs, vs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_DSS_VP(name, rs, dss, vs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetDepthStencilState(dss, 0); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_VGP(name, vs, gs, ps) \
pass name \
{ \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetGeometryShader(CompileShader(gs_5_0, gs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_VGP(name, rs, vs, gs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetGeometryShader(CompileShader(gs_5_0, gs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_DSS_VGP(name, rs, dss, vs, gs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetDepthStencilState(dss, 0); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetGeometryShader(CompileShader(gs_5_0, gs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

///////////////////////////////////////////////////////////////////////////////

#define P_V(name, vs) \
pass name \
{ \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(NULL); \
}

#define P_VP(name, vs, ps) \
pass name \
{ \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_VP(name, rs, vs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_BS_VP(name, bs, vs, ps) \
pass name \
{ \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_DSS_VP(name, rs, dss, vs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetDepthStencilState(dss, 0); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_BS_VP(name, rs, bs, vs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_VGP(name, vs, gs, ps) \
pass name \
{ \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetGeometryShader(CompileShader(gs_5_0, gs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_VGP(name, rs, vs, gs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetGeometryShader(CompileShader(gs_5_0, gs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_DSS_VGP(name, rs, dss, vs, gs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetDepthStencilState(dss, 0); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetGeometryShader(CompileShader(gs_5_0, gs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}
