float4x4 ViewProjectionMatrix : register(c0);

void VS(float3 Position : POSITION,
		float2 TexCoord : TEXCOORD0,
		out float4 OutPosition : POSITION,
		out float2 OutTexCoord : TEXCOORD0)
{	
	OutPosition = mul(float4(Position, 1), ViewProjectionMatrix);
	OutTexCoord = TexCoord;
}