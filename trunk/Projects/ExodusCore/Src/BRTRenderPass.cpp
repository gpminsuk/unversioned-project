#include "StdAfx.h"

#include "RResource.h"

#include "BViewport.h"
#include "BRTRenderPass.h"
#include "BDriver.h"

BRTRenderPass::BRTRenderPass() {
}

BRTRenderPass::~BRTRenderPass() {
}

void BRTRenderPass::BeginPass(BViewport* InViewport) {
    __super::BeginPass(InViewport);

	GDriver->SetVertexDeclaration(RVertexDeclaration::Position_TexCoord);

    RMaterial* Material = RMaterialTable::Materials(0);	
    GDriver->SetFillMode(FillMode_Solid);
	GDriver->SetRenderTarget(0, GDriver->GetBackBuffer());
	GDriver->SetTexture(0, RPR->m_BaseSceneRT->m_pTexture);
    Material->FindShader(RVertexProtocol::Protocols(1), RShaderPass::ShaderPasses(0))->BeginShader();
}

void BRTRenderPass::DrawPrimitive() {
    struct VD {
        TVector3 Pos;
        TVector2 UV;
    };

    VD Vertices[4];

    Vertices[0].Pos = TVector3(-1.0f, -1.0f, 0.0f);
    Vertices[1].Pos = TVector3(-1.0f, 1.0f, 0.0f);
    Vertices[2].Pos = TVector3(1.0f, -1.0f, 0.0f);
    Vertices[3].Pos = TVector3(1.0f, 1.0f, 0.0f);

    Vertices[0].UV = TVector2(0.0f, 1.0f);
    Vertices[1].UV = TVector2(0.0f, 0.0f);
    Vertices[2].UV = TVector2(1.0f, 1.0f);
    Vertices[3].UV = TVector2(1.0f, 0.0f);

    TIndex16 Indices[2];
    Indices[0] = TIndex16(0, 1, 2);
    Indices[1] = TIndex16(1, 3, 2);

    GDriver->DrawIndexedPrimitiveUP(PrimitiveType_TriangleList, 4, 2, Indices,
                                    sizeof(TIndex16) / 3, Vertices, sizeof(VD));
}
