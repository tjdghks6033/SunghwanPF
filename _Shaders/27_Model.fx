#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Model.fx"

float4 PS(MeshOutput input) : SV_Target0
{
    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
	
	clip(diffuse.a - 0.5f);
	
    float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);

	//float4 color = float4(diffuse * NdotL, 1);
	
	//float depthValue;
	
	//if(color.a > 0.8f)
	//{
	//	depthValue = input.sPosition.z / input.sPosition.w;
	//}
	//else
	//{
	//	discard;
	//}
	
    return float4(diffuse.rgb * NdotL, 1);
	//return float4(depthValue, depthValue, depthValue, 1);
}

technique11 T0
{
    P_VP(P0, VS_Mesh, PS)
    P_RS_VP(P1, CullMode_None,VS_Model, PS)
}