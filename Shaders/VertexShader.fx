float4x4 ViewProjectionMatrix : register(c0);
float4x4 LocalToWorld : register(c1);

void VS(float3 Position : POSITION,
		float2 TexCoord : TEXCOORD0,
		float3 Normal : NORMAL,	
		out float4 OutPosition : POSITION,
		out float2 OutTexCoord : TEXCOORD0,
		out float3 OutNormal : TEXCOORD1,
		out float3 OutPixelPosition : TEXCOORD2
		)
{	
	OutPosition = mul(float4(Position, 1), ViewProjectionMatrix);
	OutTexCoord = TexCoord;
	OutNormal = Normal;
	OutPixelPosition = Position;
}