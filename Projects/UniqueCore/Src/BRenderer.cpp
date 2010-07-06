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
	Viewport->SortTemplates();
	m_pBuffer = GDriver->CreatePrimitiveBuffer(&Viewport->m_Batches);

	m_OpaqueBasePass->BeginPass(Viewport);
	if(m_pBuffer)
	{
		GDriver->SetTexture(0, RTextureBufferTable::TextureBuffers(0));
		m_OpaqueBasePass->DrawPrimitive(&Viewport->m_Batches);
		GDriver->SetTexture(0, NULL);

		m_pBuffer->Release();
		delete m_pBuffer;
	}


	m_pBuffer = GDriver->CreatePrimitiveBuffer(&Viewport->m_LineBatch);
	if(m_pBuffer)
	{
		m_OpaqueBasePass->DrawPrimitive(&Viewport->m_LineBatch);

		m_pBuffer->Release();
		delete m_pBuffer;
	}

	m_OpaqueBasePass->EndPass();

	m_ParticleRenderPass->BeginPass(Viewport);

	m_pBuffer = GDriver->CreatePrimitiveBuffer(&Viewport->m_ParticleBatch);
	if(m_pBuffer)
	{
		GDriver->SetRenderTarget(0, m_OpaqueBasePass->m_RenderTargets(0));
		m_ParticleRenderPass->DrawPrimitive(&Viewport->m_ParticleBatch);

		m_pBuffer->Release();
		delete m_pBuffer;
	}

	m_ParticleRenderPass->EndPass();

	m_BaseRTRenderPass->BeginPass(Viewport);
	m_BaseRTRenderPass->DrawPrimitive();
	m_BaseRTRenderPass->EndPass();
	
	m_DrawLinePass->BeginPass(Viewport);
	m_DrawLinePass->DrawPrimitive(LineBatcher);
	m_DrawLinePass->EndPass();

	m_DrawFontPass->BeginPass(Viewport);
	m_pBuffer = GDriver->CreatePrimitiveBuffer(&Viewport->m_UIBatches);
	if(m_pBuffer)
	{
		GDriver->SetRenderTarget(0, m_OpaqueBasePass->m_RenderTargets(0));
		m_DrawFontPass->DrawPrimitive();

		m_pBuffer->Release();
		delete m_pBuffer;
	}
	m_DrawFontPass->EndPass();
	return true;
}


void BRenderer::SyncThread()
{
	for(UINT i=0;i<m_Viewports.Size();++i)
	{
		BViewport* Viewport = m_Viewports[i];
		BPrimitive* Primitive;
		for(UINT i=0;i<Viewport->m_OpaquePrimitives.Size();++i)
		{
			Primitive = Viewport->m_OpaquePrimitives[i];
			Primitive->RenderThreadSyncronizer->SyncData();
		}
		for(UINT i=0;i<Viewport->m_TranslucentPrimitives.Size();++i)
		{
			Primitive = Viewport->m_TranslucentPrimitives[i];
			Primitive->RenderThreadSyncronizer->SyncData();
		}
		for(UINT i=0;i<Viewport->m_UIPrimitives.Size();++i)
		{
			Primitive = Viewport->m_UIPrimitives[i];
			Primitive->RenderThreadSyncronizer->SyncData();
		}
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