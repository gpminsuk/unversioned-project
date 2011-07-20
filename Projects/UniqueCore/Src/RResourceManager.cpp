#include "StdAfx.h"

#include "CBoxPrimitive.h"
#include "CCylinderPrimitive.h"

#include "RResourceManager.h"

#include "RDXResource.h"

#include "BDriver.h"
#include "BSoundDriver.h"

RResourceManager::RResourceManager(void)
{
}

RResourceManager::~RResourceManager(void)
{
}
#include <Windows.h>
bool RResourceManager::LoadResources()
{
	TString Filename;
	//////////////////////////////////// Sound Loading
	RTexture* Texture = new RTexture();
	GDriver->CreateTextureBuffer(256, 256);

	Filename = "..\\..\\Resources\\terrain.jpg";
	extern RTextureBuffer* GDefaultTexture;
	GDefaultTexture = GDriver->CreateTextureBuffer(Filename);

	//////////////////////////////////// Shader Loading
	RDirectXShader *pShader = new RDirectXShader();
	//sprintf_s(pShader->m_FileName, 256, "Shader.fx");
	wsprintf(pShader->m_FileName, TEXT("Shader.fx"));
	RShaderTable::Shaders.AddItem(pShader);

	pShader = new RDirectXShader();
	//sprintf_s(pShader->m_FileName, 256, "RTShader.fx");
	wsprintf(pShader->m_FileName, TEXT("RTShader.fx"));
	RShaderTable::Shaders.AddItem(pShader);

	for(unsigned int i=0;i<RShaderTable::Shaders.Size();i++)
	{
		GDriver->CompileShaderFromFile(RShaderTable::Shaders(i));
	}

	//////////////////////////////////// Geometry Loading
	/////////////////////////////////////////////////////// Basic Geometry Creating
	extern TBoxPrimitive* GBoxPrimitiveOutside;
	GBoxPrimitiveOutside = new TBoxPrimitive(RenderType_Opaque, SideType_Outside);
	extern TBoxPrimitive* GBoxPrimitiveInside;
	GBoxPrimitiveInside = new TBoxPrimitive(RenderType_Opaque, SideType_Inside);
	extern TCylinderPrimitive* GCylinderPrimitive;
	GCylinderPrimitive = new TCylinderPrimitive(RenderType_Opaque);
	extern TCylinderPrimitive* GCylinderPrimitiveWireFrame;
	GCylinderPrimitiveWireFrame= new TCylinderPrimitive(RenderType_Line);
	/////////////////////////////////////////////////////// Vertex Buffer Loading
	/////////////////////////////////////////////////////// Index Buffer Loading
	return true;
}

bool RResourceManager::ReleaseAllResources()
{
	//////////////////////////////////// Geometry Releasing
	/////////////////////////////////////////////////////// Basic Geometry Releasing
	extern TBoxPrimitive* GBoxPrimitiveOutside;
	delete GBoxPrimitiveOutside;
	extern TBoxPrimitive* GBoxPrimitiveInside;
	delete GBoxPrimitiveInside;
	extern TCylinderPrimitive* GCylinderPrimitive;
	delete GCylinderPrimitive;
	extern TCylinderPrimitive* GCylinderPrimitiveWireFrame;
	delete GCylinderPrimitiveWireFrame;

	//////////////////////////////////// Shader Releasing
	for(unsigned int i=0;i<RShaderTable::Shaders.Size();++i)
	{
		delete RShaderTable::Shaders(i);
	}
	RShaderTable::Shaders.Clear(true);

	return true;
}