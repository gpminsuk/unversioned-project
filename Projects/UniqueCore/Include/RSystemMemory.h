#pragma once

#include "AObject.h"
#include "TDataTypes.h"
#include "RResource.h"

class RSystemMemoryVertexBuffer: public AObject
{
DECLARE_CLASS(RSystemMemoryVertexBuffer,)
	public:
	RSystemMemoryVertexBuffer() :
			pVertices(0), nVertices(0) {
	}
	virtual ~RSystemMemoryVertexBuffer()
	{
		delete[] pVertices;
	}

	unsigned int nVertices;

	RVertexProtocol* Protocol;

	char *pVertices;

	virtual bool Access(AAccessor& Accessor);
};

class RSystemMemoryIndexBuffer: public AObject
{
DECLARE_CLASS(RSystemMemoryIndexBuffer,)
	public:
	RSystemMemoryIndexBuffer() :
			pIndices(0), nIndices(0) {
	}
	virtual ~RSystemMemoryIndexBuffer()
	{
		delete[] pIndices;
	}

	TIndex16 *pIndices;
	unsigned int nIndices;

	virtual bool Access(AAccessor& Accessor);
};

class RDynamicPrimitiveBuffer: public RPrimitiveBuffer
{
public:
	RDynamicPrimitiveBuffer() :
			m_pVB(0), m_pIB(0) {
	}
	virtual ~RDynamicPrimitiveBuffer() {
	}

	RVideoMemoryVertexBuffer* m_pVB;
	RVideoMemoryIndexBuffer* m_pIB;

	virtual void Release() = 0;
};

class RStaticPrimitiveBuffer: public RPrimitiveBuffer
{
public:
	RSystemMemoryVertexBuffer* m_pVB;
	RSystemMemoryIndexBuffer* m_pIB;
};
