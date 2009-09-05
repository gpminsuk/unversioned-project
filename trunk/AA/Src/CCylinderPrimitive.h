#pragma once

#include "BPrimitive.h"

class TCylinderPrimitive : public TPrimitive
{
public:
	TCylinderPrimitive();
};

class CCylinderPrimitive : public BPrimitive
{
public:
	CCylinderPrimitive(void);
	virtual ~CCylinderPrimitive(void);

	virtual void Render(TBatch *Batch);
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();
};

extern TCylinderPrimitive* GCylinderPrimitive;
