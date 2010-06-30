#pragma once

class BRTRenderPass
{
public:
	BRTRenderPass(RRenderTarget* RT);
	~BRTRenderPass();

	void DrawPrimitive();

	void BeginPass(BViewport* Viewport);
	void EndPass();

	RRenderTarget*	m_RenderTarget;
};