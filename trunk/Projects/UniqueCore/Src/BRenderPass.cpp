#include "StdAfx.h"

#include "BRenderPass.h"
#include "BDriver.h"

#include "RResource.h"

BRenderPassResource GRenderPassResource;

BRenderPassResource::BRenderPassResource() :
	m_BaseSceneRT(0)
{
}

BRenderPassResource::~BRenderPassResource()
{
	if(m_BaseSceneRT)
	{
		m_BaseSceneRT->Release();
		delete m_BaseSceneRT;
	}
}

void BRenderPassResource::Initialize()
{
	m_BaseSceneRT = GDriver->CreateRenderTarget(800, 600, PixelFormat_A8R8G8B8);
}

BRenderPass::BRenderPass()
: RPR(&GRenderPassResource)
{
}

BRenderPass::~BRenderPass()
{
}