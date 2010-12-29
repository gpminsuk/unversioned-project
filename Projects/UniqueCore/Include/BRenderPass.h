#pragma once

class RShaderBase;
class BViewport;
class RRenderTarget;
class BRenderingBatchChunk;

class BRenderPassResource
{
public:
	BRenderPassResource();
	~BRenderPassResource();

	RRenderTarget*	m_BaseSceneRT;

	void Initialize();
};

class BRenderPass
{
public:
	BRenderPass();
	~BRenderPass();

	virtual void BeginPass(BViewport* InViewport);
	virtual void EndPass();

	virtual void BeginRenderBatch(BRenderingBatchChunk* BatchChunk);
	virtual void EndRenderBatch();

	BRenderPassResource* RPR;
	BViewport* Viewport;
	RShaderBase* Shader;
};

extern BRenderPassResource GRenderPassResource;