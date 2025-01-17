///////////////////////////////////////////////////////////////////////////////
//Water
///////////////////////////////////////////////////////////////////////////////
cbuffer CB_Water
{
	float4 RefractionColor;
	float2 NormalMapTile;

	float WaveTranslation;
	float WaveScale;
	float WaterShininess;
	float WaterAlpha;
};

struct VertexOutput_Water
{
	float4 Position : SV_Position;
	float3 wPosition : Position1;
	float4 ReflectionPosition : Position2;
	float4 RefractionPosition : Position3;
    
	float2 Uv : Uv;
	float2 Uv2 : Uv1;
};

VertexOutput_Water VS_Water(VertexTexture input)
{
	VertexOutput_Water output;
    
	output.Position = WorldPosition(input.Position);
	output.wPosition = output.Position.xyz;

	output.Position = ViewProjection(output.Position);
    
    
	output.ReflectionPosition = WorldPosition(input.Position);
	output.ReflectionPosition = mul(output.ReflectionPosition, Reflection);
	output.ReflectionPosition = mul(output.ReflectionPosition, Projection);
    
	output.RefractionPosition = output.Position;
    
    
	output.Uv = input.Uv / NormalMapTile.x;
	output.Uv2 = input.Uv / NormalMapTile.y;
     
	
	return output;
	
}

float4 PS_Water(VertexOutput_Water input) : SV_Target
{
	input.Uv.y += WaveTranslation;
	input.Uv2.x += WaveTranslation;
    
	float4 normalMap = NormalMap.Sample(LinearSampler, input.Uv) * 2.0f - 1.0f;
	float4 normalMap2 = NormalMap.Sample(LinearSampler, input.Uv2) * 2.0f - 1.0f;
    
	float3 normal = normalMap.rgb + normalMap2.rgb;
    
	
	
	float2 reflection;
	reflection.x = input.ReflectionPosition.x / input.ReflectionPosition.w * 0.5f + 0.5f;
	reflection.y = -input.ReflectionPosition.y / input.ReflectionPosition.w * 0.5f + 0.5f;	
	reflection = reflection + (normal.xy * WaveScale);
	float4 reflectionColor = ReflectionMap.Sample(LinearSampler, reflection);
    //return float4(reflectionColor.rgb, 1.0f);
    
	float2 refraction;
	refraction.x = input.RefractionPosition.x / input.RefractionPosition.w * 0.5f + 0.5f;
	refraction.y = -input.RefractionPosition.y / input.RefractionPosition.w * 0.5f + 0.5f;
	refraction = refraction + (normal.xy * WaveScale);
	float4 refractionColor = saturate(RefractionMap.Sample(LinearSampler, refraction) + RefractionColor);
    //return float4(refractionColor.rgb, 1.0f);
    
    
	float3 light = GlobalLight.Direction;
	light.y *= -1.0f;
	light.z *= -1.0f;
    
	float3 view = normalize(ViewPosition() - input.wPosition);
	float3 heightView = view.yyy;
    
	float r = (1.2f - 1.0f) / (1.2f + 1.0f);
	float fresnel = max(0, min(1, r + (1 - r) * pow(1 - dot(normal, heightView), 2)));
    
    //float3 diffuse = float3(0, 0, 1);
	float3 diffuse = lerp(reflectionColor, refractionColor, fresnel).rgb;
    
    
	float3 R = normalize(reflect(light, normal));
	float specular = saturate(dot(R, view));
    
    [flatten]
	if (specular > 0.0f)
	{
		specular = pow(specular, WaterShininess);
		diffuse = saturate(diffuse + specular);
	}
    
	//float4 color = CalcualteFogColor(float4(diffuse, 1), input.wPosition);
	float4 color = float4(diffuse, 1);
	return float4(color.rgb, WaterAlpha);
}

VertexTexture VS_Trail(VertexTexture input)
{
	VertexTexture output;
	
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);
	
	output.Uv = input.Uv;
	
	return output;
}

float4 PS_Trail(VertexTexture input) : SV_Target
{
	float4 color2 = TrailMap.Sample(PointSampler, input.Uv);
	
	return color2;
}