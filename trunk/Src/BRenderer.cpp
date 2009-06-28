#include "stdafx.h"
#include "BRenderer.h"
#include "BViewport.h"
#include "BApplication.h"
#include "BDriver.h"

BRenderer::BRenderer(void)
:	m_fFPS(0.f),
	m_iFTimeIdx(0),
	m_ppViewports(0),
	m_nViewportCount(0)
{
	for(int i=0;i<100;++i)
		m_dFrameTime[i] = 0;

	m_nViewportCount = 1;
	m_ppViewports = new BViewport*[5];
}

void BRenderer::AddViewport(BViewport* pViewport)
{
	m_ppViewports[0] = pViewport;
}

bool BRenderer::Initialize()
{
	m_pDriver->CreateDriver();

	m_pBox = new TBox(TVector3(1.f,1.f,1.f));
	m_pBox->pShader = RShaderTable::pShaders;

	m_ppViewports[0]->Render(m_pBox);

	m_pQuad = new TQuad();
	m_pQuad->pShader = RShaderTable::pShaders;
	
	m_ppViewports[0]->Render(m_pQuad);
	
	m_pTexture = m_pDriver->CreateTextureBuffer();
	return true;
}



bool BRenderer::Destroy()
{
	m_pTexture->DestroyTextureBuffer();

	m_pDriver->DestroyDriver();

	delete m_pBox;
	m_pBox = 0;
	delete m_pQuad;
	m_pQuad = 0;
	return true;
}

bool BRenderer::Render()
{
	RenderViewport(m_ppViewports[0]);	
	return true;
}

bool BRenderer::RenderViewport(BViewport* Viewport)
{
	Viewport->SortTemplates();
	m_pBuffer = m_pDriver->CreatePrimitiveBuffer(&Viewport->m_Batches);

	Viewport->m_BasePrimitives[0]->pShader->BeginShader();

	m_pDriver->SetTexture(0, m_pTexture);

	for(unsigned int i=0;i<Viewport->m_BasePrimitives[0]->pShader->m_nPass;++i)
	{		
		Viewport->m_BasePrimitives[0]->pShader->BeginPass();
		Viewport->m_BasePrimitives[0]->pShader->SetParameter(i);
		
		m_pDriver->DrawPrimitive();

		Viewport->m_BasePrimitives[0]->pShader->EndPass();
	}


	Viewport->m_BasePrimitives[0]->pShader->EndShader();

	m_pBuffer->DestroyVertexBuffer();
	delete m_pBuffer;
	return true;
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