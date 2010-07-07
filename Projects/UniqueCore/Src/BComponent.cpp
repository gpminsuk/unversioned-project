#include "StdAfx.h"
#include "BComponent.h"
#include "BPrimitive.h"
#include "BSynchronizer.h"

BComponent::BComponent()
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

void BComponent::SendSyncData()
{
	SyncronizerRef->SendData(this);
}