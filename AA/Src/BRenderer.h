#pragma once

#include "AThread.h"
#include <mmsystem.h>
#include "RResource.h"
#include "TDataTypes.h"

#pragma comment(lib,"winmm.lib")

class BViewport;
class BPrimitiveBuffer;
class BTextureBuffer;
class BApplication;

class BRenderer : public AThread
{
public:
	BRenderer();
	~BRenderer();

	bool Initialize();
	bool Destroy();
	bool Render();
	bool RenderViewport(BViewport* Viewport);

	void ThreadSetup();
	void ThreadExecute();
	void ThreadDestroy();

	void AddViewport(BViewport* pViewport);
	void FetchViewports();

	inline void SetApplication(BApplication *App) { m_pApp = App; }
protected:
	TArray<BViewport*> m_RendererViewport;
	TArray<BViewport*> m_Viewports;

	class BOpaqueBasePass*	m_OpaqueBasePass;
	class BRTRenderPass*	m_BaseRTRenderPass;

	BApplication* m_pApp;
	BPrimitiveBuffer* m_pBuffer;
	int m_nViewportCount;

	DWORD m_dFrameTime[100];
	int m_iFTimeIdx;
	float m_fFPS;
};
