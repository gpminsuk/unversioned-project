float4x4 ViewProjectionMatrix : register(c0);

struct TVertexInput
{
	float3 Position			: POSITION;
	float2 TexCoord			: TEXCOORD0;
	float3 Normal			: NORMAL;
};

struct TVertexOutput
{
	float2 TexCoord			: TEXCOORD0;
	float3 Normal			: TEXCOORD1;
	float3 PixelPosition	: TEXCOORD2;
};

float4 CalcOutPosition(in TVertexInput VertexInput)
{
	return mul(float4(VertexInput.Position, 1), ViewProjectionMatrix);
}

void VS(
	in TVertexInput VertexInput,
	out TVertexOutput VertexOutput,
	out float4 OutPosition : POSITION)
{
	OutPosition = CalcOutPosition(VertexInput);
	VertexOutput.TexCoord = VertexInput.TexCoord;
	VertexOutput.Normal = VertexInput.Normal;
	VertexOutput.PixelPosition = VertexInput.Position;
}