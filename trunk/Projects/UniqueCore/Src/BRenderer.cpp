#include "stdafx.h"
#include "BRenderer.h"
#include "BViewport.h"
#include "AApplication.h"
#include "BDriver.h"
#include "BSoundDriver.h"

#include "BOpaqueBasePass.h"
#include "BRTRenderPass.h"
#include "BDrawLinePass.h"
#include "BDrawUIPass.h"
#include "BParticleRenderPass.h"
#include "BRenderingBatch.h"

#include "BPrimitive.h"
#include "BLineBatcher.h"

BOpaqueBasePass*		GOpaqueBasePass;
BRTRenderPass*			GBaseRTRenderPass;
BDrawLinePass*			GDrawLinePass;	
BDrawUIPass*			GDrawFontPass;
BParticleRenderPass*	GParticleRenderPass;

BRenderer::BRenderer(AApplication *App)
:	m_fFPS(0),
	m_iFTimeIdx(0),
	m_nViewportCount(0),
	AThread(App)
{
	for(int i=0;i<FPS_COUNTER_NUMBER;++i)
		m_dFrameTime[i] = 0;

	m_nViewportCount = 1;

	GOpaqueBasePass = new BOpaqueBasePass();
	GBaseRTRenderPass = new BRTRenderPass(GOpaqueBasePass->m_RenderTargets(0));
	GDrawLinePass = new BDrawLinePass();
	GDrawFontPass = new BDrawUIPass();
	GParticleRenderPass = new BParticleRenderPass();

	LineBatcher = new BLineBatcher();
}

BRenderer::~BRenderer()
{
	delete LineBatcher;	
	delete GParticleRenderPass;
	delete GDrawFontPass;
	delete GDrawLinePass;
	delete GOpaqueBasePass;
	delete GBaseRTRenderPass;

	delete GDriver;
	delete GSoundDriver;
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
	Viewport->BatchManager->RenderBatches(Viewport);

	GBaseRTRenderPass->BeginPass(Viewport);
	GBaseRTRenderPass->DrawPrimitive();
	GBaseRTRenderPass->EndPass();
	return true;
}

bool BRenderer::Syncronize()
{
	for(UINT i=0;i<m_Viewports.Size();++i)
	{
		SyncronizeViewport(m_Viewports[i]);	
	}
	return true;
}

bool BRenderer::SyncronizeViewport(BViewport* Viewport)
{
	Viewport->BatchManager->Syncronize();
	return true;
}

void BRenderer::ThreadSetup()
{
	if(!Initialize()) Application->bQuit = true;
}

void BRenderer::ThreadExecute()
{
	DWORD dTime = timeGetTime();
	int ddd = 0;
	int Count = 0;
	int AccumulatedCount = 0;
	while(!Application->bQuit)
	{
		Syncronize();

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
		Sleep(1);
		break;
	}
	
}

void BRenderer::ThreadDestroy()
{
	Destroy();
	Application->bRenderThreadQuit = true;
}