#include "StdAfx.h"

#include "BRenderPass.h"
#include "BRenderingBatch.h"
#include "BDriver.h"

#include "RResource.h"

BRenderPassResource GRenderPassResource;

BRenderPassResource::BRenderPassResource() :
    m_BaseSceneRT(0) {
}

BRenderPassResource::~BRenderPassResource() {
    if (m_BaseSceneRT) {
        m_BaseSceneRT->Release();
        delete m_BaseSceneRT;
    }
}

void BRenderPassResource::Initialize() {
    m_BaseSceneRT = GDriver->CreateRenderTarget(800, 600, PixelFormat_A8R8G8B8);
}

BRenderPass::BRenderPass()
    :
    RPR(&GRenderPassResource) {
}

BRenderPass::~BRenderPass() {
}

void BRenderPass::BeginRenderBatch(BRenderingBatch* Batch) {
	GDriver->SetVertexDeclaration(Batch->Declaration);

    Shader = Batch->Shader;
    Shader->BeginShader();
    Shader->SetParameter(Viewport);
}

void BRenderPass::EndRenderBatch() {
    Shader->EndShader();
    Shader = 0;
}

void BRenderPass::BeginPass(BViewport* InViewport) {
    Viewport = InViewport;
}

void BRenderPass::EndPass() {
    Viewport = 0;
}
