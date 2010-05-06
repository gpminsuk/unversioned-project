#pragma once

#include "TDataTypes.h"
#include "BComponent.h"

class CTerrainComponent : public BComponent
{
public:
	CTerrainComponent(class BThing* InOwner, class CCamera* Camera);
	virtual ~CTerrainComponent();

	class CCamera* CurrentCamera;

	virtual void UpdateComponent();
};
