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

	RShaderBase* Shader;

	RDynamicPrimitiveBuffer* PrimitiveBuffer;

	TArray<BPrimitive*> Primitives;
	TArray<BLightComponent*> Lights;

	enum EPrimitiveType PrimitiveType;
	enum ERenderType RenderType;

	BRenderingBatch();
	~BRenderingBatch();

	int GetNumIndices();
	void IndexTessellate();

	void ConfigureShader();
	void RenderBaseScene();
	void RenderLight();
};

class BRenderingBatchManager
{
public:
	BRenderingBatchManager();
	~BRenderingBatchManager();

	TArray<BRenderingBatch*> Batches;

	void AddPrimitive(BPrimitive* Primitive);
	void RemovePrimitive(BPrimitive* Primitive);

	void RenderBatches(BViewport* Viewport);
};