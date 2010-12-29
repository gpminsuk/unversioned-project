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
class AApplication;

struct TRendererInfo
{
public:
	TRendererInfo() {}
};

class BRenderer : public AThread
{
public:
	BRenderer(AApplication *App);
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
protected:
	TArray<BViewport*> m_Viewports;
	class BLineBatcher* LineBatcher;
	int m_nViewportCount;

	DWORD m_dFrameTime[FPS_COUNTER_NUMBER];
	int m_iFTimeIdx;
	DWORD m_fFPS;
};

extern class BOpaqueBasePass*		GOpaqueBasePass;
extern class BDirectionalLightPass*	GDirectionalLightPass;
extern class BRTRenderPass*			GBaseRTRenderPass;
extern class BDrawLinePass*			GDrawLinePass;	
extern class BDrawUIPass*			GDrawFontPass;
extern class BParticleRenderPass*	GParticleRenderPass;

