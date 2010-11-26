#pragma once

#include "AObject.h"
#include "TDataTypes.h"

class BSynchronizer;

class BComponent : public AObject
{
public:
	BComponent();
	virtual ~BComponent();

	TArray<class BPrimitive*> Primitives;

	virtual void UpdateComponent() = 0;
};