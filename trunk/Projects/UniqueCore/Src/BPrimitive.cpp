#include "StdAfx.h"
#include "BPrimitive.h"
#include "BSynchronizer.h"

#include "RResource.h"

BPrimitive::BPrimitive()
: Syncronizer(0)
{
}

BPrimitive::~BPrimitive(void)
{
	Primitives.Clear();
	delete Syncronizer;
}

void BPrimitive::Syncronize()
{
	Syncronizer->RecieveData(this);
}

TPrimitive::TPrimitive()
: pBuffer(0)
{
}

TPrimitive::~TPrimitive()
{
	delete pBuffer;
}