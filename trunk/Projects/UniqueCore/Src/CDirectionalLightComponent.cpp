#include "stdafx.h"
#include "CDirectionalLightComponent.h"

#include "RResource.h"

CDirectionalLightComponent::CDirectionalLightComponent() {

}

CDirectionalLightComponent::~CDirectionalLightComponent() {

}

void CDirectionalLightComponent::UpdateComponent() {

}

void CDirectionalLightComponent::ConfigureShader(RShader* Shader) {
    float Radius = 200.0f;
    TVector3 Position(0.0f, 50.0f, 0.0f);
    float Power = 5.0f;
    Shader->SetShaderConstantF("LightRadius", &Radius);
    Shader->SetShaderConstantF("LightPosition", (float*) &Position);
    Shader->SetShaderConstantF("LightBrightness", &Power);
}
