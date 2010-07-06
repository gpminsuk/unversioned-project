#pragma once

#include "AObject.h"

#include "TDataTypes.h"
#include "RResource.h"
#include "UFreeTypeDrawer.h"

class BSynchronizer;

enum ERenderType
{
	RenderType_Opaque,
	RenderType_Translucent,
	RenderType_Particle,
	RenderType_Line,
	RenderType_UI,
};

class TPrimitive
{
public:
	TPrimitive();
	virtual ~TPrimitive();

	virtual void UpdatePrimitive() {}

	class RStaticPrimitiveBuffer* pBuffer;
};

class BPrimitive : public AObject
{
public:
	BPrimitive() {}
	BPrimitive(BSynchronizer** Synchronizer);
	virtual ~BPrimitive(void);

	TVector3 Translation;
	TQuaternion Rotation;
	TVector3 Scale;

	TMatrix TM;

	TArray<TPrimitive*> Primitives;
	unsigned int NumIndices;

	struct TBatchInfo
	{
		short BatchNum;
		short SlotNum;
	};

	TBatchInfo BatchInfo;

	enum ERenderType RenderType;

	BSynchronizer* RenderThreadSyncronizer;

	virtual void UpdatePrimitive() {}

	virtual void InitializeSynchronizer(BSynchronizer** Synchronizer) {}
	virtual void Render(TBatch *Batch) = 0;
	virtual unsigned int FillDynamicVertexBuffer(char** pData) = 0;
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex) = 0;
	virtual unsigned int GetNumIndices() = 0;
	virtual void IndexTessellate() {};
};
