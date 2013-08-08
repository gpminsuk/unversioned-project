#pragma once

#include "AObject.h"
#include "TDataTypes.h"

class RAnimationNode: public AObject {
	DECLARE_CLASS(RAnimationNode,)
public:
	TArray<RAnimationNode*> Children;
};

class RAnimationController: public AObject
{
DECLARE_CLASS(RAnimationController,)
public:
	RAnimationNode Root;
};
