#include "StdAfx.h"

#include "RResource.h"

#include "BOpaqueBasePass.h"
#include "BDriver.h"

BOpaqueBasePass::BOpaqueBasePass()
{

}

BOpaqueBasePass::~BOpaqueBasePass()
{

}

void BOpaqueBasePass::DrawPrimitive(BViewport* Viewport, TPrimitiveTemplateBase *Prim)
{
	RShaderBase* pShader = RShaderTable::pShaders;
	pShader->BeginShader();
	pShader->SetParameter(Viewport);
	GDriver->DrawPrimitive();
	pShader->EndShader();
}