struct VertexInput
{
	float4 Position : Position;
	float2 Scale : Scale;
	float Random : Random;

	uint VertexID : SV_VertexID;
};

struct VertexOutput
{
	float4 Position : Position;
	float2 Scale : Scale;
	float Random : Random;

	uint VertexID : VertexID;
};

VertexOutput VS_Billboard(VertexInput input)
{
	VertexOutput output;


    //input.Position = float4(input.VertexID * 2.0f, 0, 0, 1);
    //input.Scale = float2(1, 1);

	output.Position = WorldPosition(input.Position);
	output.Scale = input.Scale;
	output.Random = input.Random;
    
	output.VertexID = input.VertexID;

	return output;
}

struct BillboardDepthOutput
{
	float4 Position : SV_Position0;
	float2 Scale : Scale;
	float4 sPosition : Position1;
	uint VertexID : VertexID;	
};

BillboardDepthOutput VS_BillboardDepth(VertexInput input)
{
	BillboardDepthOutput output;
	
	output.Position = WorldPosition(input.Position);
	output.Position = mul(output.Position, ShadowView);
	output.Position = mul(output.Position, ShadowProjection);
	
	output.Scale = input.Scale;
	
	output.sPosition = output.Position;
	output.VertexID = input.VertexID;
	return output;
}

struct GeometryOutput
{
	float4 Position : SV_Position;
	float2 Uv : Uv;

	uint VertexID : VertexID;
};


float uvrandom;

[maxvertexcount(4)]
void GS_Billboard(point VertexOutput input[1], inout TriangleStream<GeometryOutput> stream)
{
	float3 up = float3(0, 1, 0);
    //float3 forward = input[0].Position.xyz - ViewPosition();
	float3 forward = float3(0, 0, 1);
	float3 right = normalize(cross(up, forward));

	float2 size = input[0].Scale * 0.5f;

	float4 position[4];
	position[0] = float4(input[0].Position.xyz - size.x * right - size.y * up, 1);
	position[1] = float4(input[0].Position.xyz - size.x * right + size.y * up, 1);
	position[2] = float4(input[0].Position.xyz + size.x * right - size.y * up, 1);
	position[3] = float4(input[0].Position.xyz + size.x * right + size.y * up, 1);


	float2 uv[4] =
	{
		float2(0, 1), float2(0, 0), float2(1, 1), float2(1, 0)
	};


	GeometryOutput output;
    
    //[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		output.Position = ViewProjection(position[i]);
		if (i == 1 || i == 3)
			output.Uv = uv[i] + uvrandom;
		else
			output.Uv = uv[i];
		output.VertexID = input[0].VertexID;

		stream.Append(output);
	}
}

[maxvertexcount(4)]
void GS_BillboardDepth(point BillboardDepthOutput input[1], inout TriangleStream<GeometryOutput> stream)
{
	float3 up = float3(0, 1, 0);
    //float3 forward = input[0].Position.xyz - ViewPosition();
	float3 forward = float3(0, 0, 1);
	float3 right = normalize(cross(up, forward));

	float2 size = input[0].Scale * 0.5f;

	float4 position[4];
	position[0] = float4(input[0].Position.xyz - size.x * right - size.y * up, 1);
	position[1] = float4(input[0].Position.xyz - size.x * right + size.y * up, 1);
	position[2] = float4(input[0].Position.xyz + size.x * right - size.y * up, 1);
	position[3] = float4(input[0].Position.xyz + size.x * right + size.y * up, 1);


	float2 uv[4] =
	{
		float2(0, 1), float2(0, 0), float2(1, 1), float2(1, 0)
	};


	GeometryOutput output;
    
    //[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		output.Position = ViewProjection(position[i]);
		if (i == 1 || i == 3)
			output.Uv = uv[i] + uvrandom;
		else
			output.Uv = uv[i];
		output.VertexID = input[0].VertexID;

		stream.Append(output);
	}
}

Texture2DArray Maps;

float4 PS_BillboardDepth(GeometryOutput input) : SV_Target
{
	float depth = input.Position.z / input.Position.w;
    
	return float4(depth, depth, depth, 1.0f);
}

float4 PS_Billboard(GeometryOutput input) : SV_Target
{
	float4 diffuse = Maps.Sample(LinearSampler, float3(input.Uv, input.VertexID % 6));

	return diffuse;
}
