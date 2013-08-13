#pragma once

#include "TDataTypes.h"
#include "RSystemMemory.h"

class BPrimitive;
class BLineBatcher;
class BLight;

class BRenderingBatch
{
public:
	int nVertices;
	RVertexProtocol* Protocol;
	RMaterial* pMaterial;

	RDynamicPrimitiveBuffer* PrimitiveBuffer;

	TArray<BPrimitive*> Primitives;
	TArray<BLightComponent*> Lights;

	enum EPrimitiveType PrimitiveType;
	enum ERenderType RenderType;

	BRenderingBatch(BPrimitive* InitialPrimitive);
	~BRenderingBatch();

	bool IsBatchable(BPrimitive* Primitive);
	void RemovePrimitive(BPrimitive* Primitive);
	void BatchPrimitive(BPrimitive* Primitive);
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

	BLineBatcher* LineBatcher;
	TArray<BLightComponent*> Lights;
	TArray<BViewport*>* m_Viewports;
	TArray<BRenderingBatch*> Batches;

	void AddPrimitive(BPrimitive* Primitive);
	void RemovePrimitive(BPrimitive* Primitive);

	void Render();
	void RenderBatches();
};
