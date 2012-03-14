#include "PSCommon.ush"
#include "StaticMeshVertexProtocol.ush"
#include "Material.ush"

#define POINT_LIGHT
#ifdef POINT_LIGHT
#include "PointLight.ush"
#endif

#ifdef DIRECTIONAL_LIGHT
#include "DirectionalLight.ush"
#endif

void PS(in TVertexOutput VertexOutput,
		out float4 OutColor : COLOR0)
{
	OutColor = GetDiffuse(VertexOutput);
#ifdef POINT_LIGHT
	OutColor *= GetPointLight(VertexOutput); 
#endif
}