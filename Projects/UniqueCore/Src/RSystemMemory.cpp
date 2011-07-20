#include "stdafx.h"
#include "RSkeletalMesh.h"

IMPLEMENT_CLASS(RSystemMemoryIndexBuffer);
IMPLEMENT_CLASS(RSystemMemoryVertexBuffer);

bool RSystemMemoryIndexBuffer::Access(AAccessor& Accessor)
{
	Accessor << nIndices;
	if(Accessor.IsLoading())
	{
		pIndices = new TIndex16[nIndices];
	}
	for(unsigned int i=0;i<nIndices;++i)
	{
		Accessor << pIndices[i];
	}
	return true;
}

bool RSystemMemoryVertexBuffer::Access(AAccessor& Accessor)
{
	Accessor << nVertexStride;
	Accessor << nVertices;
	if(Accessor.IsLoading())
	{
		pVertices = new char[nVertexStride*nVertices];
	}
	for(unsigned int i=0;i<nVertices;++i)
	{
		Accessor << pVertices[i];
	}	
	return true;
}