#include "stdafx.h"
#include "BRenderer.h"
#include "BViewport.h"
#include "BApplication.h"
#include "BDriver.h"

#include "BOpaqueBasePass.h"
#include "BRTRenderPass.h"
#include "BDrawLinePass.h"
#include "BDrawUIPass.h"
#include "BParticleRenderPass.h"
#include "BRenderingBatch.h"

#include "BPrimitive.h"
#include "BSynchronizer.h"
#include "BLineBatcher.h"

BRenderer::BRenderer(BApplication *App)
:	m_fFPS(0),
	m_iFTimeIdx(0),
	m_nViewportCount(0),
	m_pApp(App)
{
	for(int i=0;i<FPS_COUNTER_NUMBER;++i)
		m_dFrameTime[i] = 0;

	m_nViewportCount = 1;

	m_OpaqueBasePass = new BOpaqueBasePass();
	m_BaseRTRenderPass = new BRTRenderPass(m_OpaqueBasePass->m_RenderTargets(0));
	m_DrawLinePass = new BDrawLinePass();
	m_DrawFontPass = new BDrawUIPass();
	m_ParticleRenderPass = new BParticleRenderPass();

	LineBatcher = new BLineBatcher();
}

BRenderer::~BRenderer()
{
	delete LineBatcher;	
	delete m_ParticleRenderPass;
	delete m_DrawFontPass;
	delete m_DrawLinePass;
	delete m_OpaqueBasePass;
	delete m_BaseRTRenderPass;

	delete GDriver;
	GDriver = 0;
}

void BRenderer::AddViewport(BViewport* pViewport)
{
	m_Viewports.AddItem(pViewport);
}

bool BRenderer::Initialize()
{
	return true;
}

bool BRenderer::Destroy()
{
	GDriver->DestroyDriver();
	return true;
}

bool BRenderer::Render()
{
	for(UINT i=0;i<m_Viewports.Size();++i)
	{
		RenderViewport(m_Viewports[i]);	
	}
	return true;
}

bool BRenderer::RenderViewport(BViewport* Viewport)
{
	Viewport->RenderingBatches->RenderBatch();
	return true;
}


void BRenderer::SyncThread()
{
	for(UINT i=0;i<m_Viewports.Size();++i)
	{
		m_Viewports(i)->RenderingBatches->Syncronize();
	}	
}

void BRenderer::ThreadSetup()
{
	if(!Initialize()) m_pApp->bQuit = true;
}

void BRenderer::ThreadExecute()
{
	DWORD dTime = timeGetTime();
	int ddd = 0;
	int Count = 0;
	int AccumulatedCount = 0;
	while(!m_pApp->bQuit)
	{
		SyncThread();

		if(GDriver->BeginScene())
		{
			Render();
			GDriver->EndScene();
		}
		
		m_fFPS -= m_dFrameTime[m_iFTimeIdx];
		m_dFrameTime[m_iFTimeIdx] = timeGetTime() - dTime;
		if(m_dFrameTime[m_iFTimeIdx] != 0)
		{
			m_fFPS += m_dFrameTime[m_iFTimeIdx];
			dTime = timeGetTime();
		}
		if(++m_iFTimeIdx >= FPS_COUNTER_NUMBER) m_iFTimeIdx = 0;
	}
}

void BRenderer::ThreadDestroy()
{
	Destroy();
	m_pApp->bRenderThreadQuit = true;
}