#pragma once

#include "AObject.h"

#include "TDataTypes.h"

enum ERenderType
{
	RenderType_Opaque,
	RenderType_Translucent,
	RenderType_Particle,
	RenderType_Line,
};

class TPrimitive
{
public:
	TPrimitive();
	virtual ~TPrimitive();

	class RStaticPrimitiveBuffer* pBuffer;
};

class BPrimitive : public AObject
{
public:
	BPrimitive();
	virtual ~BPrimitive(void);

	TVector3 Translation;
	TQuaternion Rotation;
	TVector3 Scale;

	TMatrix TM;

	TArray<TPrimitive*> Primitives;

	struct TBatchInfo
	{
		short BatchNum;
		short SlotNum;
	};

	TBatchInfo BatchInfo;

	enum ERenderType RenderType;

	virtual void Render(TBatch *Batch) = 0;
	virtual unsigned int FillDynamicVertexBuffer(char** pData) = 0;
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex) = 0;
	virtual unsigned int GetNumIndices() = 0;
	virtual void IndexTessellate() {};
};
