#pragma once

#include "TDataTypes.h"

class BSynchronizer;

class BComponent
{
public:
	BComponent();
	virtual ~BComponent();

	BSynchronizer* Syncronizer;

	TArray<class BPrimitive*> Primitives;

	virtual void UpdateComponent() = 0;
};