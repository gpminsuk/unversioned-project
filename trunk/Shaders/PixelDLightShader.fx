texture Texture0;
float3 EyeVector : register(c0);
float3 LightPosition : register(c1);
float3 LightDirection : register(c2);
float LightBrightness : register(c3);
float LightRadius : register(c4);

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
	float4 Ambient = {0.0f,0.0f,0.0f,0.0f};
	
	float3 LightVector = (LightPosition - PixelPosition);
	float Distance = length(LightVector);
	LightVector = normalize(LightVector);
	float4 Diffuse = (DiffuseColor*LightBrightness*((LightRadius-Distance)/LightRadius));
	
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
	
	if(Distance < LightRadius)
		OutColor = Diffuse;
	//OutColor.a = 1.0f;
	//OutColor = float4(Distance/LightRadius,Distance/LightRadius,Distance/LightRadius,1);
	//OutColor = Diffuse + Ambient;
	//OutColor = float4(Normal.x, Normal.y, Normal.z, 1);
	//OutColor = float4(0.0f,0.0f,0.0f, 1);
	//OutColor = float4(LightBrightness, LightRadius, 0.0f ,1);
}