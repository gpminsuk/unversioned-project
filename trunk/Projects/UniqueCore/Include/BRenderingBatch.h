#pragma once

#include "TDataTypes.h"
#include "RResource.h"

class BPrimitive;
class BLight;

class BRenderingBatch
{
public:
	int nVertices;
	int nVertexStride;

	RDynamicPrimitiveBuffer* PrimitiveBuffer;

	TArray<BPrimitive*> Primitives;

	enum EPrimitiveType PrimitiveType;
	enum ERenderType RenderType;

	BRenderingBatch();
	~BRenderingBatch();

	int GetNumIndices();
	void IndexTessellate();

	void RenderBatch(BViewport* Viewport);
	void Syncronize();
};

class BRenderingBatchManager
{
public:
	BRenderingBatchManager();
	~BRenderingBatchManager();

	TArray<BRenderingBatch*> RenderingBatches;

	void AddPrimitive(BPrimitive* Component);
	void RemovePrimitive(BPrimitive* Component);
	void RenderBatches(BViewport* Viewport);
	void Syncronize();
};