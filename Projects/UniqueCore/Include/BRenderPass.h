#pragma once

class RRenderTarget;

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

	BRenderPassResource* RPR;
};