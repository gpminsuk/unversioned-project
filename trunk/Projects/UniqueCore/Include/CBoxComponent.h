#pragma once

#include "TDataTypes.h"
#include "BComponent.h"

class CBoxComponent : public BComponent
{
public:
	CBoxComponent(EGeometrySideType _BoxSideType = SideType_Outside);
	virtual ~CBoxComponent();

	virtual void UpdateComponent();
};
