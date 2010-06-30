#pragma once

#include "TDataTypes.h"

class BComponent
{
public:
	BComponent(class BThing* InOwner);
	virtual ~BComponent();

	TArray<class BPrimitive*> Primitives;

	class BThing* Owner;

	virtual void UpdateComponent() = 0;
};
