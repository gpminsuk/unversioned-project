#include "StdAfx.h"
#include "CDirectionalLight.h"

#include "CDirectionalLightComponent.h"

CDirectionalLight::CDirectionalLight(void) {
    CDirectionalLightComponent* DirectionalLightComponent =
        new CDirectionalLightComponent();
    Components.AddItem(DirectionalLightComponent);
}

CDirectionalLight::~CDirectionalLight(void) {
}

bool CDirectionalLight::Tick(unsigned long dTime) {
    return true;
}

void CDirectionalLight::PhysicsTick(unsigned long dTime) {
}

void CDirectionalLight::UpdateTransform() {
}
