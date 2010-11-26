#pragma once

#include "TDataTypes.h"
#include "BComponent.h"

class CBoxComponent : public BComponent
{
public:
	CBoxComponent();
	virtual ~CBoxComponent();

	virtual void UpdateComponent();
};
