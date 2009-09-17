texture Texture0;
float3 EyeVecto2r : register(c0);

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
	float4 DiffuseColor = tex2D(samp, TexCoord);
	float4 Ambient = {0.1f,0.1f,0.1f,0.1f};
	float3 LightPosition = {6,2.5,6};
	float LightRadius = 20.0f;
	float LightBrightness = 1.4f;
	
	float3 LightVector = (LightPosition - PixelPosition);
	float Distance = length(LightVector);
	LightVector = normalize(LightVector);
	float4 Diffuse = (saturate(dot(LightVector,Normal))*DiffuseColor*LightBrightness*((LightRadius-Distance)/LightRadius));
	
	float3 EyePosition = {6,3,6};
	float SpecularPower = 10;
	float SpecularBrightness = 0.3f;
	float3 ReflectionVector = 2*dot(Normal,LightVector)*Normal - LightVector;
	float3 EyeVector = normalize(EyePosition - PixelPosition);	
	float SpecularValue = pow(saturate(dot(EyeVector, ReflectionVector)),SpecularPower);
	if(dot(Normal,LightVector) <= 0)
		SpecularValue = 0;
	else
		SpecularValue = SpecularValue*SpecularBrightness;
	float4 Specular = {SpecularValue,SpecularValue,SpecularValue,0};
	
	
	if(Distance > LightRadius)
		OutColor = Ambient;
	else
		OutColor = Specular + Diffuse + Ambient;
}