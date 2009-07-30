#include "StdAfx.h"
#include "RResourceManager.h"

#include "RDXResource.h"

#include "BDriver.h"

RResourceManager::RResourceManager(void)
{
}

RResourceManager::~RResourceManager(void)
{
}

bool RResourceManager::LoadResources()
{
	RTextureBuffer *pTexBuffer = GDriver->CreateTextureBuffer();
	RTextureBufferTable::pTextureBuffer.AddItem(pTexBuffer);

	RDirectXShader *pShader = new RDirectXShader();
	sprintf_s(pShader->m_FileName, 256, "Shader.fx");
	RShaderTable::pShaders.AddItem(pShader);

	pShader = new RDirectXShader();
	sprintf_s(pShader->m_FileName, 256, "RTShader.fx");
	RShaderTable::pShaders.AddItem(pShader);

	for(unsigned int i=0;i<RShaderTable::pShaders.Size();++i)
	{
		GDriver->CompileShaderFromFile(RShaderTable::pShaders(i));
	}
	return true;
}

bool RResourceManager::ReleaseAllResources()
{
	for(unsigned int i=0;i<RShaderTable::pShaders.Size();++i)
	{
		delete RShaderTable::pShaders(i);
	}
	RShaderTable::pShaders.Clear(true);

	for(unsigned int i=0;i<RTextureBufferTable::pTextureBuffer.Size();++i)
	{
		delete RTextureBufferTable::pTextureBuffer(i);		
	}
	RTextureBufferTable::pTextureBuffer.Clear(true);
	return true;
}