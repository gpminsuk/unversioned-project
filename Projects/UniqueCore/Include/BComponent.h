#pragma once

#include "TDataTypes.h"

class BSynchronizer;

class BComponent
{
public:
	BComponent();
	virtual ~BComponent();

	TArray<class BPrimitive*> Primitives;

	virtual void UpdateComponent() = 0;
};