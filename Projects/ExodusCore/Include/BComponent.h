#pragma once

#include "AObject.h"
#include "TDataTypes.h"
#include "Enumerations_Geom.h"

class BSynchronizer;
class BRenderingBatchManager;
class BRenderer;

class BComponent: public AObject
{
public:
	BComponent();
	virtual ~BComponent();

	TArray<class BPrimitive*> Primitives;

	virtual void RenderComponent(BRenderingBatchManager* BatchManager, BRenderer* Renderer);
	virtual void UpdateComponent() = 0;
};
