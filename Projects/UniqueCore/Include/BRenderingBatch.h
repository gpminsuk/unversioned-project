#pragma once

#include "TDataTypes.h"
#include "RResource.h"

class BPrimitive;

enum ERenderingPass
{
	RenderingPass_Opaque = 0,
	RenderingPass_Line,
	RenderingPass_UI,
	RenderingPass_Particle,
	RenderingPass_RenderTarget
};

class BRenderingBatch
{
public:
	int nVertices;
	int nVertexStride;

	RDynamicPrimitiveBuffer* PrimitiveBuffer;

	TArray<BPrimitive*> Primitives;

	enum EPrimitiveType RenderType;
	ERenderingPass RenderingPassType;

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
	TArray<BPrimitive*> RenderPrimitives;

	void AddPrimitive(BPrimitive* Component);
	void RenderBatches(BViewport* Viewport);
	void Syncronize();
};