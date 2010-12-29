#pragma once

#include "BLightComponent.h"

class CDirectionalLightComponent : public BLightComponent
{
public:
	CDirectionalLightComponent();
	~CDirectionalLightComponent();

	virtual void UpdateComponent();
};