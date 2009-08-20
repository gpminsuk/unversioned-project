#pragma once

#include "AObject.h"

#include "TDataTypes.h"

class TPrimitive
{
public:
	TPrimitive();
	~TPrimitive();

	TMatrix TM;

	class RStaticPrimitiveBuffer* pBuffer;

	virtual void Render(TBatch *Batch) = 0;
	virtual unsigned int FillDynamicVertexBuffer(char** pData) = 0;
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex) = 0;
	virtual unsigned int GetNumIndices() = 0;
};

class BPrimitive : public AObject
{
public:
	BPrimitive(void);
	~BPrimitive(void);

	TVector4 qTranslation;
	TQuaternion qRotation;
	TVector4 qScale;

	TArray<TPrimitive*> Primitives;

	struct TBatchInfo
	{
		short BatchNum;
		short SlotNum;
	};
	TBatchInfo BatchInfo;
};