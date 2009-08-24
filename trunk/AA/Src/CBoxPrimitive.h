#pragma once

#include "BPrimitive.h"

class TBoxPrimitive : public TPrimitive
{
public:
	TBoxPrimitive();

	virtual void Render(TBatch *Batch);
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();
};

class CBoxPrimitive : public BPrimitive
{
public:
	CBoxPrimitive(void);
	~CBoxPrimitive(void);
};
