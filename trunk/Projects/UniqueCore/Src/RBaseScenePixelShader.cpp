#include "stdafx.h"
#include "RShaderClasses.h"

void RBaseScenePixelShader::ConfigureShader(BPrimitive* InPrimitive)
{
	float Radius = 200.0f;
	TVector3 Position(0.0f,50.0f,0.0f);
	float Power = 1.0f;
	Shader->SetShaderConstantF("LightRadius", &Radius);
	Shader->SetShaderConstantF("LightPosition", (float*)&Position);
	Shader->SetShaderConstantF("LightBrightness", &Power);
}