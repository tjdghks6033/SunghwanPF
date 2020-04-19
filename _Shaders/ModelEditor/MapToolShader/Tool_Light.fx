struct MaterialDesc
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
};

cbuffer CB_Material
{
    MaterialDesc Material;
};

struct LightDesc
{
    float4 Ambient;
    float4 Specular;
    float3 Direction;
    float Padding;
    float3 Position;
};

cbuffer CB_Light
{
    LightDesc GlobalLight; //Ambient Light
};
void ComputeLight(inout float4 color, float3 normal, float3 wPosition)
{
    float4 ambient = 0;
    float4 diffuse = 0;
    float4 specular = 0;

    float3 direction = -GlobalLight.Direction;
    float NdotL = dot(direction, normalize(normal));
    
    
    ambient = GlobalLight.Ambient * Material.Ambient;

    [flatten]
    if (NdotL > 0.0f)
    {
        diffuse = NdotL * Material.Diffuse;

        [flatten]
        if (any(Material.Specular.rgb) && any(Material.Specular.a))
        {
            wPosition = ViewPosition() - wPosition;

            float3 R = normalize(reflect(-direction, normal));
            float RdotE = saturate(dot(R, normalize(wPosition)));
            
            float shininess = pow(RdotE, Material.Specular.a);
            specular = shininess * Material.Specular * GlobalLight.Specular;
        }
    }

    color = ambient + diffuse + specular;
}

void NormalMapping(float2 uv, float3 normal, float3 tangent, SamplerState samp)
{
    float4 map = NormalMap.Sample(samp, uv);

    [flatten]
    if (any(map) == false)
        return;


    //탄젠트 공간
    float3 N = normalize(normal); //Z
    float3 T = normalize(tangent - dot(tangent, N) * N); //X
    float3 B = cross(N, T); //Y;
    float3x3 TBN = float3x3(T, B, N);

    //이미지로부터 계산된 노멀
    float3 coord = map.rgb * 2.0f - 1.0f;

    //탄젠트 공간으로 변환
    coord = mul(coord, TBN);

    Material.Diffuse *= saturate(dot(coord, -GlobalLight.Direction));
}

void NormalMapping(float2 uv, float3 normal, float3 tangent)
{
    NormalMapping(uv, normal, tangent, LinearSampler);
}


void InstanceNormalMapping(float3 uvw, float3 normal, float3 tangent, SamplerState samp)
{
    float4 map = NormalMaps.Sample(samp, uvw);

    [flatten]
    if (any(map) == false)   //노말맵이 없으면 리턴
        return;

    //탄젠트 공간
    float3 N = normalize(normal); //z
    float3 T = normalize(tangent - dot(tangent, N) * N);
    float3 B = cross(N, T); //y
    float3x3 TBN = float3x3(T, B, N);

    //이미지로 부터 계산된 노멀
    float3 coord = map.rgb * 2.0f - 1.0f;
    
    //탄젠트 공간으로 변환
    coord = mul(coord, TBN);

    Material.Diffuse *= saturate(dot(coord, -GlobalLight.Direction));
}

void InstanceNormalMapping(float3 uvw, float3 normal, float3 tangent)
{
    InstanceNormalMapping(uvw, normal, tangent, LinearSampler);

}

///////////////////////////////////////////////////////////////////////////////

// 추가적으로 사용할려면 디퍼드 랜더링 ?

#define MAX_POINT_LIGHT 32

struct PointLightDesc
{
    float4 Color;
    float3 Position;
    float Range;
    float Intensity;

    float3 Padding;
};

cbuffer CB_PointLight
{
    uint PointLightCount;
    float3 CB_PointLight_Padding;

    PointLightDesc PointLights[MAX_POINT_LIGHT];

};

void ComputePointLights(inout float4 color,  float3 wPosition)
{
    //Material.Specular = Material.Diffuse;
    //[unroll(MAX_POINT_LIGHT)]
    for (uint i = 0; i < PointLightCount; i++)
    {
        float dist = distance(PointLights[i].Position.xyz, wPosition.xyz);

        [flatten]
        if(dist > 0.0f)
        {
            float att = saturate((PointLights[i].Range - dist) / PointLights[i].Range);
            att = pow(att, PointLights[i].Intensity);

            //                                     빛이 들어갈곳과 안들어 갈곳 
            color += PointLights[i].Color * att * Material.Specular;
        }
    }

}


///////////////////////////////////////////////////////////////////////////////

// 추가적으로 사용할려면 디퍼드 랜더링 ?

#define MAX_SPOT_LIGHT 32

struct SpotLightDesc
{
    float4 Color;

    float3 Position;
    float Range;

    float3 Direction;
    float Angle;

    float Intensity;

    float3 Padding;

};

cbuffer CB_SpotLight
{
    uint SpotLightCount;
    float3 CB_SpotLight_Padding;

    SpotLightDesc SpotLights[MAX_POINT_LIGHT];

};

void ComputeSpotLights(inout float4 color, float3 wPosition)
{
   //[unroll(MAX_SPOT_LIGHT)]
    for (uint i = 0; i < SpotLightCount; i++)
    {
        float3 dist = SpotLights[i].Position - wPosition;
    
        [flatten]
        if (length(dist) < SpotLights[i].Range)
        {
            float3 direction = normalize(SpotLights[i].Position - wPosition);

            float angle = dot(-SpotLights[i].Direction, direction);

            [flatten]
            if (angle > 0.0f)
            {
                // max : 0보다 큰값 
                float intensity = max(dot(-dist, SpotLights[i].Direction),0);
                float att = pow(intensity, SpotLights[i].Angle);

                color += SpotLights[i].Color * att * Material.Specular;

            }
        }
  
    }

}
///////////////////////////////////////////////////////////////////////////////


