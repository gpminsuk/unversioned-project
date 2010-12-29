#pragma once

#include "TDataTypes.h"
#include "RResource.h"

class BPrimitive;
class BLight;
class BRenderingBatch;

class BRenderingBatchChunk
{
public:
	TArray<BRenderingBatch*> Batches;

	RShaderBase* Shader;

	void RenderBaseScene();
	void RenderLight();

	void AddPrimitive(BPrimitive* Primitive);
	void RemovePrimitive(BPrimitive* Primitive);
};

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

	void RenderBaseScene();
	void RenderLight();
};

class BRenderingBatchManager
{
public:
	BRenderingBatchManager();
	~BRenderingBatchManager();

	TArray<BRenderingBatchChunk*> BatchChunks;

	void AddPrimitive(BPrimitive* Primitive);
	void RemovePrimitive(BPrimitive* Primitive);

	void RenderBatchChunks(BViewport* Viewport);
};