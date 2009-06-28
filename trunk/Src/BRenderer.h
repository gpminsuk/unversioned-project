#pragma once

#include "AThread.h"
#include <mmsystem.h>
#include "RResource.h"
#include "TDataTypes.h"

#pragma comment(lib,"winmm.lib")

class BViewport;
class BDriver;
class BPrimitiveBuffer;
class BTextureBuffer;
class BApplication;

class BRenderer : public AThread
{
public:
	BRenderer();

	bool Initialize();
	bool Destroy();
	bool Render();
	bool RenderViewport(BViewport* Viewport);

	void ThreadSetup();
	void ThreadExecute();
	void ThreadDestroy();

	void AddViewport(BViewport* pViewport);

	inline void SetDriver(BDriver* Driver) { m_pDriver = Driver; }
	inline void SetApplication(BApplication *App) { m_pApp = App; }

protected:
	TBox *m_pBox;
	TQuad *m_pQuad;

	BApplication* m_pApp;
	BDriver* m_pDriver;
	BPrimitiveBuffer* m_pBuffer;
	BTextureBuffer* m_pTexture;
	BViewport** m_ppViewports;
	int m_nViewportCount;

	DWORD m_dFrameTime[100];
	int m_iFTimeIdx;
	float m_fFPS;
};
