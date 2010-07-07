#pragma once

#include "TDataTypes.h"

class BSynchronizer;

class BComponent
{
public:
	BComponent();
	virtual ~BComponent();

	BSynchronizer* SyncronizerRef;

	TArray<class BPrimitive*> Primitives;

	void SendSyncData();
	virtual void UpdateComponent() = 0;
};