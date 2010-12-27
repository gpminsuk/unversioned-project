texture Texture0;

sampler samp = sampler_state
{
    Texture   = (Texture0);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

void PS(float2 TexCoord : TEXCOORD0,
		float3 Normal : TEXCOORD1,
		float3 PixelPosition : TEXCOORD2,
		out float4 OutColor : COLOR0)
{
	float4 DiffuseColor = {0.3f,0.3f,0.3f,0.3f};
	float4 Ambient = {0.2f,0.2f,0.2f,0.0f};
		
	OutColor = DiffuseColor + Ambient;	
}