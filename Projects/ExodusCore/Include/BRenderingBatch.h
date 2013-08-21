#pragma once

#include "TDataTypes.h"
#include "RSystemMemory.h"

class BPrimitive;
class BLineBatcher;
class BLight;

class RTextureBuffer;

class BRenderingBatch
{
public:
	int nVertices;
	RVertexProtocol* Protocol;
	RMaterial* pMaterial;
	bool isDirty;

	RDynamicPrimitiveBuffer* PrimitiveBuffer;

	RTextureBuffer* Texture;
	TArray<BPrimitive*> Primitives;
	TArray<BLight*> Lights;

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
	TArray<BLight*> Lights;
	TArray<BViewport*>* m_Viewports;
	TArray<BRenderingBatch*> Batches;

	void AddPrimitive(BPrimitive* Primitive);
	void RemovePrimitive(BPrimitive* Primitive);

	void Render();
	void RenderBatches();
};
