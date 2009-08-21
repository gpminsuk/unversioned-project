#include "StdAfx.h"
#include "BPrimitive.h"

#include "RResource.h"

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

TPrimitive::TPrimitive()
: pBuffer(0)
{
}

TPrimitive::~TPrimitive()
{
	delete pBuffer->m_pIB;
	delete pBuffer->m_pVB;
	delete pBuffer;
}