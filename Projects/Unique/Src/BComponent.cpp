#include "StdAfx.h"
#include "BComponent.h"
#include "BPrimitive.h"

BComponent::BComponent(BThing* InOwner)
:	Owner(InOwner)
{
}

BComponent::~BComponent()
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		delete Primitives(i);
	}
	Primitives.Clear();
}