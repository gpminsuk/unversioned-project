#include "stdafx.h"
#include "BRenderer.h"
#include "BViewport.h"
#include "AApplication.h"
#include "BDriver.h"
#include "BSoundDriver.h"
#include "BThing.h"

#include "BCollisionBody.h"
#include "BComponent.h"
#include "BTextDrawer.h"
#include "BOpaqueBasePass.h"
#include "BRTRenderPass.h"
#include "BDrawLinePass.h"
#include "BDrawUIPass.h"
#include "BParticleRenderPass.h"
#include "BRenderingBatch.h"
#include "BDirectionalLightPass.h"

#include "BPrimitive.h"
#include "BLineBatcher.h"

BOpaqueBasePass* GOpaqueBasePass;
BDirectionalLightPass* GDirectionalLightPass;
BRTRenderPass* GBaseRTRenderPass;
BDrawLinePass* GDrawLinePass;
BDrawUIPass* GDrawFontPass;
BParticleRenderPass* GParticleRenderPass;

BRenderer::BRenderer(AApplication *App)
    :
    m_fFPS(0),
    m_iFTimeIdx(0),
	m_Viewports(App->Viewports),
    AThread(App) {
    for (int i = 0; i < FPS_COUNTER_NUMBER; ++i)
        m_dFrameTime[i] = 0;
	
    GOpaqueBasePass = new BOpaqueBasePass();
    GBaseRTRenderPass = new BRTRenderPass();
    GDrawLinePass = new BDrawLinePass();
    GDrawFontPass = new BDrawUIPass();
    GParticleRenderPass = new BParticleRenderPass();
    GDirectionalLightPass = new BDirectionalLightPass();

    LineBatcher = new BLineBatcher();
	BatchManager = new BRenderingBatchManager();
    GTextDrawer = new BTextDrawer();
}

BRenderer::~BRenderer() {
    delete GTextDrawer;
    delete LineBatcher;
    delete GParticleRenderPass;
    delete GDrawFontPass;
    delete GDrawLinePass;
    delete GOpaqueBasePass;
    delete GBaseRTRenderPass;
    delete GDirectionalLightPass;

    delete GDriver;
    delete GSoundDriver;
    GDriver = 0;

    delete BatchManager;
}

bool BRenderer::Initialize() {
    return true;
}

bool BRenderer::Destroy() {
    GDriver->DestroyDriver();
    return true;
}

bool BRenderer::Render() {
	for (UINT i = 0; i < m_Viewports.Size(); ++i) {
		BViewport* Viewport = m_Viewports(i);
		GDriver->BeginScene(Viewport);

		BatchManager->RenderBatches(Viewport, Lights);

		GBaseRTRenderPass->BeginPass(Viewport);
		GBaseRTRenderPass->DrawPrimitive();
		GBaseRTRenderPass->EndPass();
		char f[30];
		float ff = 1000.0f/(m_fFPS/FPS_COUNTER_NUMBER);
		sprintf_s(f, "%f", ff);
		GTextDrawer->AddText(TString(f));
		GTextDrawer->DrawTexts(Viewport);

		GDriver->EndScene(Viewport);
    }
    return true;
}

void BRenderer::ThreadSetup() {
    if (!Initialize())
        Application->bQuit = true;
}

void BRenderer::ThreadExecute() {
    DWORD dTime = timeGetTime();
    int ddd = 0;
    int Count = 0;
    int AccumulatedCount = 0;
    //while(!Application->bQuit)
    //{
	Render();

    m_fFPS -= m_dFrameTime[m_iFTimeIdx];
    m_dFrameTime[m_iFTimeIdx] = timeGetTime() - dTime;
    if (m_dFrameTime[m_iFTimeIdx] != 0) {
        m_fFPS += m_dFrameTime[m_iFTimeIdx];
        dTime = timeGetTime();
    }
    if (++m_iFTimeIdx >= FPS_COUNTER_NUMBER)
        m_iFTimeIdx = 0;
    //	Sleep(1);
    //}

}

void BRenderer::ThreadDestroy() {
    Destroy();
    Application->bRenderThreadQuit = true;
}

void BRenderer::Render(BPrimitive* pPrimitive) {
    BatchManager->AddPrimitive(pPrimitive);
}

void BRenderer::Remove(BPrimitive* pPrimitive) {
    BatchManager->RemovePrimitive(pPrimitive);
}

void BRenderer::Remove(BThing* pThing) {
    for (unsigned int i = 0; i < pThing->Components.Size(); ++i) {
        BComponent* pComponent = pThing->Components(i);
        for (unsigned int j = 0; j < pComponent->Primitives.Size(); ++j) {
            Remove(pComponent->Primitives(j));
        }
    }
    for (unsigned int i = 0; i < pThing->CollisionBodies.Size(); ++i) {
        BCollisionBody* pCollisionBody = pThing->CollisionBodies(i);
        for (unsigned int j = 0; j < pCollisionBody->Primitives.Size(); ++j) {
            Remove(pCollisionBody->Primitives(j));
        }
    }
}

void BRenderer::Render(BThing* pThing) {
	for (unsigned int i = 0; i < pThing->Components.Size(); ++i) {
		BComponent* pComponent = pThing->Components(i);
		pComponent->RenderComponent(this);
	}
	for (unsigned int i = 0; i < pThing->CollisionBodies.Size(); ++i) {
		BCollisionBody* pCollisionBody = pThing->CollisionBodies(i);
		for (unsigned int j = 0; j < pCollisionBody->Primitives.Size(); ++j) {
			Render(pCollisionBody->Primitives(j));
		}
	}
}

void BRenderer::RenderLight(BLightComponent* pLightComponent) {
	Lights.AddItem(pLightComponent);
}

void BRenderer::RemoveLight(BLightComponent* pLightComponent) {
	Lights.DeleteItemByVal(pLightComponent);
}