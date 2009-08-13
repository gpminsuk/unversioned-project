#include "StdAfx.h"
#include "BThing.h"

#include "BPrimitive.h"

BThing::BThing(void)
{
}

BThing::~BThing(void)
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		delete Primitives(i);
	}
	Primitives.Clear();
}
