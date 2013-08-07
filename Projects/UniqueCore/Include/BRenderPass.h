#pragma once

class RShaderBase;
class BViewport;
class RRenderTarget;
class BRenderingBatch;
class RMaterial;

class BRenderPassResource
{
public:
	BRenderPassResource();
	~BRenderPassResource();

	RRenderTarget* m_BaseSceneRT;

	void Initialize();
};

class BRenderPass
{
public:
	BRenderPass();
	~BRenderPass();

	virtual void BeginPass(BViewport* InViewport);
	virtual void EndPass();

	virtual void BeginRenderBatch(BRenderingBatch* Batch);
	virtual void EndRenderBatch();

	BRenderPassResource* RPR;
	BViewport* Viewport;
	RShaderBase* Shader;
};

extern BRenderPassResource GRenderPassResource;
