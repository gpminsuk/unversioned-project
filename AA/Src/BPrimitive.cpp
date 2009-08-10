#include "StdAfx.h"
#include "BPrimitive.h"

BPrimitive::BPrimitive(void)
{
}

BPrimitive::~BPrimitive(void)
{
	for(unsigned int i=0;i<Primitives.Size();++i)
	{
		delete Primitives(i);
	}
	Primitives.Clear();
}
