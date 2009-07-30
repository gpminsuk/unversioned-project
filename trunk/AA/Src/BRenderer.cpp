#include "stdafx.h"
#include "BRenderer.h"
#include "BViewport.h"
#include "BApplication.h"
#include "BDriver.h"

#include "BOpaqueBasePass.h"
#include "BRTRenderPass.h"

BRenderer::BRenderer(void)
:	m_fFPS(0.f),
	m_iFTimeIdx(0),
	m_nViewportCount(0)
{
	for(int i=0;i<100;++i)
		m_dFrameTime[i] = 0;

	m_nViewportCount = 1;

	m_OpaqueBasePass = new BOpaqueBasePass();
	m_BaseRTRenderPass = new BRTRenderPass(m_OpaqueBasePass->m_RenderTargets(0));
}

BRenderer::~BRenderer()
{
	for(UINT i=0;i<m_RendererViewport.Size();++i)
	{
		delete m_RendererViewport(0);
		m_RendererViewport.DeleteItem(0);
	}
	
	delete m_OpaqueBasePass;
	delete m_BaseRTRenderPass;
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
	for(UINT i=0;i<m_RendererViewport.Size();++i)
		RenderViewport(m_RendererViewport[i]);	
	return true;
}

bool BRenderer::RenderViewport(BViewport* Viewport)
{
	Viewport->SortTemplates();
	m_pBuffer = GDriver->CreatePrimitiveBuffer(&Viewport->m_Batches);

	m_OpaqueBasePass->BeginPass(Viewport);
	for(UINT PrimIdx = 0;PrimIdx < Viewport->m_OpaquePrimitives.Size(); ++PrimIdx)
	{
		TPrimitiveTemplateBase* Prim = Viewport->m_OpaquePrimitives[PrimIdx];		

		GDriver->SetTexture(0, RTextureBufferTable::pTextureBuffer(0));
		m_OpaqueBasePass->DrawPrimitive(Prim);
		GDriver->SetTexture(0, NULL);
	}
	m_OpaqueBasePass->EndPass();

	m_BaseRTRenderPass->BeginPass(Viewport);
	m_BaseRTRenderPass->DrawPrimitive();
	m_BaseRTRenderPass->EndPass();

	m_pBuffer->DestroyVertexBuffer();
	delete m_pBuffer;
	return true;
}

void BRenderer::FetchViewports()
{
	int Count = (UINT)(m_Viewports.Size() - m_RendererViewport.Size());
	if(Count > 0)
	{
		for(int i=0;i<Count;++i)
		{
			BViewport *vp = new BViewport;
			m_RendererViewport.AddItem(vp);
		}
	}
	else
	{
		for(int i=0;i>Count;--i)
		{
			delete m_RendererViewport((int)(m_RendererViewport.Size()-1));
			m_RendererViewport.DeleteItem((UINT)(m_RendererViewport.Size()-1));
		}
	}
	for(UINT i=0;i<m_RendererViewport.Size();++i)
	{
		*m_RendererViewport[i] = *m_Viewports[i];
	}
}

void BRenderer::ThreadSetup()
{
	if(!Initialize()) m_pApp->bQuit = true;
}

void BRenderer::ThreadExecute()
{
	DWORD dTime;
	int ddd = 0;
	while(!m_pApp->bQuit)
	{
		FetchViewports();
		dTime = timeGetTime();


		if(GDriver->BeginScene())
		{
			Render();
			
			GDriver->EndScene();
		}
		Sleep(1);
		m_fFPS -= m_dFrameTime[m_iFTimeIdx];
		m_dFrameTime[m_iFTimeIdx] = timeGetTime() - dTime;
		m_fFPS += m_dFrameTime[m_iFTimeIdx];
		if(++m_iFTimeIdx >= 100) m_iFTimeIdx = 0;
	}
}

void BRenderer::ThreadDestroy()
{
	Destroy();
	m_pApp->bRenderThreadQuit = true;
}