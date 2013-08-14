#include "stdafx.h"
#include "BRenderer.h"
#include "BViewport.h"
#include "AApplication.h"
#include "BDriver.h"
#include "BSoundDriver.h"
#include "BThing.h"

#include "BCollisionBody.h"
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
    AThread(App) {
    for (int i = 0; i < FPS_COUNTER_NUMBER; ++i)
        m_dFrameTime[i] = 0;
	
    GOpaqueBasePass = new BOpaqueBasePass();
    GBaseRTRenderPass = new BRTRenderPass();
    GDrawLinePass = new BDrawLinePass();
    GDrawFontPass = new BDrawUIPass();
    GParticleRenderPass = new BParticleRenderPass();
    GDirectionalLightPass = new BDirectionalLightPass();

    GTextDrawer = new BTextDrawer();
}

BRenderer::~BRenderer() {
    delete GTextDrawer;
    delete GParticleRenderPass;
    delete GDrawFontPass;
    delete GDrawLinePass;
    delete GOpaqueBasePass;
    delete GBaseRTRenderPass;
    delete GDirectionalLightPass;

    delete GDriver;
    delete GSoundDriver;
    GDriver = 0;
}

bool BRenderer::Initialize() {
    return true;
}

bool BRenderer::Destroy() {
    GDriver->DestroyDriver();
    return true;
}

bool BRenderer::Render() {
	for(UINT i = 0; i < BatchManager.Size();++i) {
		BatchManager(i)->RenderBatches();
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

void BRenderer::Render(BRenderingBatchManager* BatchManager, BPrimitive* pPrimitive) {
    BatchManager->AddPrimitive(pPrimitive);
}

void BRenderer::Remove(BRenderingBatchManager* BatchManager, BPrimitive* pPrimitive) {
    BatchManager->RemovePrimitive(pPrimitive);
}

void BRenderer::Remove(BRenderingBatchManager* BatchManager, BThing* pThing) {
    for (unsigned int i = 0; i < pThing->Primitives.Size(); ++i) {
        BPrimitive* pPrimitive = pThing->Primitives(i);
        Remove(BatchManager, pPrimitive);
    }
    for (unsigned int i = 0; i < pThing->CollisionBodies.Size(); ++i) {
        BCollisionBody* pCollisionBody = pThing->CollisionBodies(i);
        for (unsigned int j = 0; j < pCollisionBody->Primitives.Size(); ++j) {
            Remove(BatchManager, pCollisionBody->Primitives(j));
        }
    }
}

void BRenderer::Render(BRenderingBatchManager* BatchManager, BThing* pThing) {
	for (unsigned int i = 0; i < pThing->Primitives.Size(); ++i) {
		BPrimitive* pPrimitive = pThing->Primitives(i);
		Render(BatchManager, pPrimitive);
	}
	for (unsigned int i = 0; i < pThing->CollisionBodies.Size(); ++i) {
		BCollisionBody* pCollisionBody = pThing->CollisionBodies(i);
		for (unsigned int j = 0; j < pCollisionBody->Primitives.Size(); ++j) {
			Render(BatchManager, pCollisionBody->Primitives(j));
		}
	}
}

void BRenderer::RenderLight(BRenderingBatchManager* BatchManager, BLight* pLight) {
	BatchManager->Lights.AddItem(pLight);
}

void BRenderer::RemoveLight(BRenderingBatchManager* BatchManager, BLight* pLight) {
	BatchManager->Lights.DeleteItemByVal(pLight);
}