#include "stdafx.h"
#include "BRenderer.h"
#include "BViewport.h"
#include "BApplication.h"
#include "BDriver.h"

BRenderer::BRenderer(void)
:	m_fFPS(0.f),
	m_iFTimeIdx(0),
	m_nViewportCount(0)
{
	for(int i=0;i<100;++i)
		m_dFrameTime[i] = 0;

	m_nViewportCount = 1;
}

void BRenderer::AddViewport(BViewport* pViewport)
{
	m_Viewports.AddItem(pViewport);
}

bool BRenderer::Initialize()
{
	m_pTexture = m_pDriver->CreateTextureBuffer();
	return true;
}

bool BRenderer::Destroy()
{
	m_pTexture->DestroyTextureBuffer();
	m_pDriver->DestroyDriver();
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
	m_pBuffer = m_pDriver->CreatePrimitiveBuffer(&Viewport->m_Batches);

	for(UINT PrimIdx = 0;PrimIdx < Viewport->m_OpaquePrimitives.Size(); ++PrimIdx)
	{
		TPrimitiveTemplateBase* Prim = Viewport->m_OpaquePrimitives[PrimIdx];
		Prim->pShader->BeginShader();

		m_pDriver->SetTexture(0, m_pTexture);

		for(UINT i=0;i<Prim->pShader->m_nPass;++i)
		{		
			Prim->pShader->BeginPass();
			Prim->pShader->SetParameter(Viewport);

			m_pDriver->DrawPrimitive();

			Prim->pShader->EndPass();
		}

		Prim->pShader->EndShader();
	}

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


		if(m_pDriver->BeginScene())
		{
			Render();
			
			m_pDriver->EndScene();
		}

		++ddd;
		if(ddd == 100)
		{
			system("cls");
			printf("FPS = %f, %d",100000.0f/(m_fFPS),m_iFTimeIdx);			
			ddd=0;
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
}