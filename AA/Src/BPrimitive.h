#pragma once

#include "AObject.h"

#include "TDataTypes.h"

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
	BPrimitive(void);
	~BPrimitive(void);

	TVector4 qTranslation;
	TQuaternion qRotation;
	TVector4 qScale;

	TMatrix TM;

	TArray<TPrimitive*> Primitives;

	struct TBatchInfo
	{
		short BatchNum;
		short SlotNum;
	};
	TBatchInfo BatchInfo;

	virtual void Render(TBatch *Batch) = 0;
	virtual unsigned int FillDynamicVertexBuffer(char** pData) = 0;
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex) = 0;
	virtual unsigned int GetNumIndices() = 0;
	virtual void IndexTessellate() {};
};