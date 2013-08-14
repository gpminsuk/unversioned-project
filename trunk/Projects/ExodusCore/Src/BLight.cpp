#include "StdAfx.h"
#include "BLight.h"
#include "RShaderClasses.h"

BLight::BLight(void) {
}

BLight::~BLight(void) {
}

void BLight::ConfigureShader(RShader* Shader) {
	float Radius = 200.0f;
	TVector3 Position(0.0f, 0.0f, 0.0f);
	float Power = 2.0f;
	Shader->SetShaderConstantF("LightRadius", &Radius);
	Shader->SetShaderConstantF("LightPosition", (float*) &Position);
	Shader->SetShaderConstantF("LightBrightness", &Power);
}