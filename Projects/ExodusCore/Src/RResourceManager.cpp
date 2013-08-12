#include "StdAfx.h"

#include "CBoxPrimitive.h"
#include "CCylinderPrimitive.h"

#include "RResourceManager.h"

#include "RDXResource.h"

#include "BDriver.h"
#include "BSoundDriver.h"

RResourceManager::RResourceManager(void) {
}

RResourceManager::~RResourceManager(void) {
}
#include <Windows.h>
bool RResourceManager::LoadResources() {
    if (!GDriver) {
        return false;
    }
    TString Filename;    
    Filename = "..\\..\\Resources\\renekton_brutal_TX_CM.DDS";
    extern RTextureBuffer* GDefaultTexture;
    GDefaultTexture = GDriver->CreateTextureBuffer(Filename);

	/////////////////////////////////////////////////////// Vertex Decl Loading
	RVertexDeclaration::Position_TexCoord = new RDXVertexDeclaration(2, DeclType_Float3, DeclUsage_Position, DeclType_Float2, DeclUsage_TexCoord);
	RVertexDeclaration::Position_Normal_TexCoord = new RDXVertexDeclaration(3, DeclType_Float3, DeclUsage_Position, DeclType_Float3, DeclUsage_Normal, DeclType_Float2, DeclUsage_TexCoord);
	RVertexDeclaration::SkeletalMesh_GPU_Skin = new RDXVertexDeclaration(5, DeclType_Float3, DeclUsage_Position, DeclType_Ubyte4, DeclUsage_BlendIndices, DeclType_Float4, DeclUsage_BlendWeight, DeclType_Float3, DeclUsage_Normal, DeclType_Float2, DeclUsage_TexCoord);

	/////////////////////////////////////////////////////// Vertex Protocol Loading
	RVertexProtocol::Protocols.AddItem(new RStaticMeshVertexProtocol());
	RVertexProtocol::Protocols.AddItem(new RRenderTargetVertexProtocol());

	/////////////////////////////////////////////////////// Shader Pass Loading
	RShaderPass::ShaderPasses.AddItem(new RBaseShaderPass());
	RShaderPass::ShaderPasses.AddItem(new RRenderTargetShaderPass());

	//////////////////////////////////// Material Loading
	RMaterialTable::Materials.AddItem(new RMaterial("Textured"));
	RMaterialTable::Materials.AddItem(new RMaterial("Red"));

    //////////////////////////////////// Geometry Loading
    /////////////////////////////////////////////////////// Basic Geometry Creating
    extern TBoxPrimitive* GBoxPrimitiveOutside;
    GBoxPrimitiveOutside = new TBoxPrimitive(RenderType_Opaque,
            SideType_Outside);
    extern TBoxPrimitive* GBoxPrimitiveInside;
    GBoxPrimitiveInside = new TBoxPrimitive(RenderType_Opaque, SideType_Inside);
    extern TCylinderPrimitive* GCylinderPrimitive;
    GCylinderPrimitive = new TCylinderPrimitive(RenderType_Opaque);
    extern TCylinderPrimitive* GCylinderPrimitiveWireFrame;
    GCylinderPrimitiveWireFrame = new TCylinderPrimitive(RenderType_Line);
    /////////////////////////////////////////////////////// Vertex Buffer Loading
    /////////////////////////////////////////////////////// Index Buffer Loading
    return true;
}

bool RResourceManager::ReleaseAllResources() {
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
    for (unsigned int i = 0; i < RMaterialTable::Materials.Size(); ++i) {
        delete RMaterialTable::Materials(i);
    }
    RMaterialTable::Materials.Clear(true);

	//////////////////////////////////// Vertex Protocol Releasing
	for (unsigned int i = 0; i < RVertexProtocol::Protocols.Size(); ++i) {
		delete RVertexProtocol::Protocols(i);
	}
	RVertexProtocol::Protocols.Clear(true);

	//////////////////////////////////// Shader Pass Releasing
	for (unsigned int i = 0; i < RShaderPass::ShaderPasses.Size(); ++i) {
		delete RShaderPass::ShaderPasses(i);
	}
	RShaderPass::ShaderPasses.Clear(true);

	//////////////////////////////////// Vertex Decl Releasing
	delete RVertexDeclaration::Position_TexCoord;
	delete RVertexDeclaration::Position_Normal_TexCoord;
	delete RVertexDeclaration::SkeletalMesh_GPU_Skin;
    return true;
}
