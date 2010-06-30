#pragma once

#include "AThread.h"
#include <mmsystem.h>
#include "RResource.h"
#include "TDataTypes.h"

#define FPS_COUNTER_NUMBER		50

#pragma comment(lib,"winmm.lib")

class BViewport;
class RPrimitiveBuffer;
class BTextureBuffer;
class BApplication;

struct TRendererInfo
{
public:
	TRendererInfo() {}
};

class BRenderer : public AThread
{
public:
	BRenderer(BApplication *App);
	~BRenderer();

	bool Initialize();
	bool Destroy();
	bool Render();
	bool RenderViewport(BViewport* Viewport);
	void RenderLines(BViewport* Viewport);

	void ThreadSetup();
	void ThreadExecute();
	void ThreadDestroy();

	void AddViewport(BViewport* pViewport);
	void FetchViewports();
protected:
	TArray<BViewport*> m_RendererViewport;
	TArray<BViewport*> m_Viewports;
	class BLineBatcher* LineBatcher;

	class BOpaqueBasePass*	m_OpaqueBasePass;
	class BRTRenderPass*	m_BaseRTRenderPass;
	class BDrawLinePass*	m_DrawLinePass;	
	class BDrawFontPass*	m_DrawFontPass;
	class BParticleRenderPass* m_ParticleRenderPass;

	BApplication* m_pApp;
	RDynamicPrimitiveBuffer* m_pBuffer;
	int m_nViewportCount;

	DWORD m_dFrameTime[FPS_COUNTER_NUMBER];
	int m_iFTimeIdx;
	DWORD m_fFPS;
};
