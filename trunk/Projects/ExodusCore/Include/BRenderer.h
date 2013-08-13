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
	
	void RenderLight(BRenderingBatchManager* BatchManager, BLightComponent* pLightComponent);
	void RemoveLight(BRenderingBatchManager* BatchManager, BLightComponent* pLightComponent);

	void Render(BRenderingBatchManager* BatchManager, BPrimitive* pPrimitive);
	void Remove(BRenderingBatchManager* BatchManager, BPrimitive* pPrimitive);	
	void Render(BRenderingBatchManager* BatchManager, BThing* pThing);
	void Remove(BRenderingBatchManager* BatchManager, BThing* pThing);

	TArray<BRenderingBatchManager*> BatchManager;
protected:

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

