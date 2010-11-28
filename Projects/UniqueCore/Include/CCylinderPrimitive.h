#pragma once

#include "BPrimitive.h"

class TCylinderPrimitive : public TPrimitive
{
public:
	TCylinderPrimitive(ERenderType _RenderType = RenderType_Opaque);
};

class CCylinderPrimitive : public BPrimitive
{
public:
	CCylinderPrimitive(ERenderType _RenderType = RenderType_Opaque);
	virtual ~CCylinderPrimitive(void);

	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();
};

extern TCylinderPrimitive* GCylinderPrimitive;
