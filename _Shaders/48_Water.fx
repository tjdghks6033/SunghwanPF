#include "00_Global.fx"
#include "00_Deffered.fx"
#include "00_Model.fx"
#include "00_Sky.fx"
#include "00_Terrain.fx"
#include "00_Water.fx"

///////////////////////////////////////////////////////////////////////////////
// PreRender
///////////////////////////////////////////////////////////////////////////////
float4 PS(MeshOutput input) : SV_Target
{
	//NormalMapping(input.Uv, input.Normal, input.Tangent);
	//Texture(Material.Diffuse, DiffuseMap, input.Uv);
	//Texture(Material.Specular, SpecularMap, input.Uv);
    
    
	//MaterialDesc result = MakeMaterial();
	//MaterialDesc output = MakeMaterial();
    
	//ComputeLight(output, input.Normal, input.wPosition);
	//AddMaterial(result, output);
        
	//float3 color = MaterialToColor(result);
	////CalcualteFogColor(float4(color, 1), input.wPosition);
    
	////return float4(color.rgb, 1.0f);
	
	
	float3 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
	float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);

	//return float4(diffuse * NdotL, 1);
	
	return PS_Shadow(input, PS_AllLight(input));	
}

technique11 T0
{ 
	//Deffered - Depth SpotLights Shadow
    P_RS_VP(P0, FrontCounterClockwise_True, VS_Depth_Mesh, PS_Depth)
    P_RS_VP(P1, FrontCounterClockwise_True, VS_Depth_Model, PS_Depth)
    P_RS_VP(P2, FrontCounterClockwise_True, VS_Depth_Animation, PS_Depth)

    //Sky
    P_VP(P3, VS_Scattering, PS_Scattering)
    P_VP(P4, VS_Dome, PS_Dome)
    P_BS_VP(P5, AlphaBlend, VS_Moon, PS_Moon)
    P_BS_VP(P6, AlphaBlend, VS_Cloud, PS_Cloud)

    //Render
    P_VP(P7, VS_Mesh, PS)
    P_VP(P8, VS_Model, PS)
    P_VP(P9, VS_Animation, PS)

    //PreRender - Water
    P_VP(P10, VS_PreRender_Reflection_Dome, PS_Dome)
    P_BS_VP(P11, AlphaBlend, VS_PreRender_Reflection_Moon, PS_Moon)
    P_BS_VP(P12, AlphaBlend, VS_PreRender_Reflection_Cloud, PS_Cloud)

    P_VP(P13, VS_PreRender_Reflection_Mesh, PS)
    P_VP(P14, VS_PreRender_Reflection_Model, PS)
    P_VP(P15, VS_PreRender_Reflection_Animation, PS)

    //Water
    P_BS_VP(P16, AlphaBlend, VS_Water, PS_Water)

	//Trail
	P_BS_VP(P17, AlphaBlend_AlphaToCoverage, VS_Trail, PS_Trail)
}