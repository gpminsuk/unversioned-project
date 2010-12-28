#pragma once

#include "AObject.h"
#include "TDataTypes.h"
#include "Enumerations_Geom.h"

class BSynchronizer;
class BViewport;

class BComponent : public AObject
{
public:
	BComponent();
	virtual ~BComponent();

	TArray<class BPrimitive*> Primitives;

	virtual void RenderComponent(BViewport* Viewport);
	virtual void UpdateComponent() = 0;
};