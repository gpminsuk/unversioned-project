#include "StdAfx.h"

#include "RResource.h"

#include "BShaderPass.h"
#include "BDriver.h"

BShaderPass::BShaderPass()
{

}

BShaderPass::~BShaderPass()
{

}

void BShaderPass::DrawPrimitive(BViewport* Viewport, TPrimitiveTemplateBase *Prim)
{
	RShaderTable::pShaders->BeginShader();
	RShaderTable::pShaders->SetParameter(Viewport);
	GDriver->DrawPrimitive();
	RShaderTable::pShaders->EndShader();
}