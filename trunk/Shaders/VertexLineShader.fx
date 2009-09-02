float4x4 ViewProjectionMatrix : register(c0);

void VS(float3 Position : POSITION,	
		out float4 OutPosition : POSITION)
{
	OutPosition = mul(float4(Position, 1), ViewProjectionMatrix);
}