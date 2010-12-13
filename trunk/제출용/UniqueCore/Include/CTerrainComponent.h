#pragma once

#include "TDataTypes.h"
#include "BComponent.h"

class CTerrainComponent : public BComponent
{
public:
	CTerrainComponent();
	virtual ~CTerrainComponent();

	virtual void UpdateComponent();
};
