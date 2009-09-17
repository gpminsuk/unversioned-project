texture Texture0;
sampler samp = sampler_state
{
    Texture   = (Texture0);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU  = CLAMP;
    AddressV  = CLAMP;
    AddressW  = CLAMP;    
};

void PS(float4 Color : COLOR0,
		float2 TexCoord : TEXCOORD0,
		out float4 OutColor : COLOR0)
{
	//OutColor = tex2D(samp, TexCoord);
	OutColor = float4(1,0,0,1);
}