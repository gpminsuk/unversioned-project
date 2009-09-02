#include "StdAfx.h"
#include "BPrimitive.h"

#include "RResource.h"

BPrimitive::BPrimitive(void)
{
}

BPrimitive::~BPrimitive(void)
{
	Primitives.Clear();
}

TPrimitive::TPrimitive()
: pBuffer(0)
{
}

TPrimitive::~TPrimitive()
{
	delete pBuffer;
}