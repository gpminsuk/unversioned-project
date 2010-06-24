#pragma once

#include "TDataTypes.h"
#include "BComponent.h"

class CTerrainComponent : public BComponent
{
public:
	CTerrainComponent(class BThing* InOwner);
	virtual ~CTerrainComponent();

	virtual void UpdateComponent();
};
