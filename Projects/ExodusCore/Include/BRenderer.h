#pragma once

#include "AThread.h"
#include <mmsystem.h>
#include "RResource.h"
#include "TDataTypes.h"

#define FPS_COUNTER_NUMBER		50

#pragma comment(lib,"winmm.lib")

class BThing;
class BViewport;
class BPrimitive;
class RPrimitiveBuffer;
class BRenderingBatchManager;
class BRenderPassResource;
class BLineBatcher;
class BTextureBuffer;
class AApplication;

struct TRendererInfo
{
public:
	TRendererInfo() {
	}
};

class BRenderer: public AThread
{
public:
	BRenderer(AApplication *App);
	~BRenderer();

	bool Initialize();
	bool Destroy();
	bool Render();
	void RenderLines(BViewport* Viewport);

	void ThreadSetup();
	void ThreadExecute();
	void ThreadDestroy();
	
	void RenderLight(BLightComponent* pLightComponent);
	void RemoveLight(BLightComponent* pLightComponent);

	void Render(BPrimitive* pPrimitive);
	void Remove(BPrimitive* pPrimitive);	
	void Render(BThing* pThing);
	void Remove(BThing* pThing);
	
	TArray<BViewport*>& m_Viewports;
protected:
	BRenderingBatchManager* BatchManager;
	BLineBatcher* LineBatcher;
	TArray<BLightComponent*> Lights;

	DWORD m_dFrameTime[FPS_COUNTER_NUMBER];
	int m_iFTimeIdx;
	DWORD m_fFPS;
};

extern class BOpaqueBasePass* GOpaqueBasePass;
extern class BDirectionalLightPass* GDirectionalLightPass;
extern class BRTRenderPass* GBaseRTRenderPass;
extern class BDrawLinePass* GDrawLinePass;
extern class BDrawUIPass* GDrawFontPass;
extern class BParticleRenderPass* GParticleRenderPass;

