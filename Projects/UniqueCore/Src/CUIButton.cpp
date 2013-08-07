#include "stdafx.h"
#include "CUIButton.h"
#include "BRenderingBatch.h"
#include "BDriver.h"

TUIButtonPrimitive::TUIButtonPrimitive() {
    pBuffer = new RStaticPrimitiveBuffer();

    RSystemMemoryVertexBuffer *pVB = new RSystemMemoryVertexBuffer();
    RSystemMemoryIndexBuffer *pIB = new RSystemMemoryIndexBuffer();
    pBuffer->m_pVB = pVB;
    pBuffer->m_pIB = pIB;

    struct VD {
        TVector3 Pos;
        TVector2 UV;
    };
	
    pVB->nVertices = 6;
    pVB->pVertices = new char[pVB->Protocol->Decl->GetStride()* pVB->nVertices];

    VD *Vertex = reinterpret_cast<VD*>(pVB->pVertices);

    Vertex[0].Pos = TVector3(-1.0f, -1.0f, 0.0f);
    Vertex[0].UV = TVector2(0.0f, 1.0f);
    Vertex[1].Pos = TVector3(1.0f, -1.0f, 0.0f);
    Vertex[1].UV = TVector2(1.0f, 1.0f);
    Vertex[2].Pos = TVector3(1.0f, 1.0f, 0.0f);
    Vertex[2].UV = TVector2(1.0f, 0.0f);
    Vertex[3].Pos = TVector3(-1.0f, -1.0f, 0.0f);
    Vertex[3].UV = TVector2(0.0f, 1.0f);
    Vertex[4].Pos = TVector3(1.0f, 1.0f, 0.0f);
    Vertex[4].UV = TVector2(1.0f, 0.0f);
    Vertex[5].Pos = TVector3(-1.0f, 1.0f, 0.0f);
    Vertex[5].UV = TVector2(0.0f, 0.0f);

    pIB->nIndices = 2;
    pIB->pIndices = new TIndex16[pIB->nIndices];

    TIndex16 *Index = reinterpret_cast<TIndex16*>(pIB->pIndices);

    Index[0] = TIndex16(0, 2, 1);
    Index[1] = TIndex16(3, 5, 4);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

CUIButtonPrimitive::CUIButtonPrimitive(RTextureBuffer* InTexture) {
    RenderType = RenderType_UI;

    Texture = InTexture;

    TUIButtonPrimitive* ButtonPrimitive = new TUIButtonPrimitive();
    Primitives.AddItem(ButtonPrimitive);
}

CUIButtonPrimitive::~CUIButtonPrimitive(void) {
}

void CUIButtonPrimitive::Render(BRenderingBatch *Batch) {
    Batch->nVertices += Primitives(0)->pBuffer->m_pVB->nVertices;
}

unsigned int CUIButtonPrimitive::GetNumIndices() {
    return Primitives(0)->pBuffer->m_pIB->nIndices;
}

unsigned int CUIButtonPrimitive::FillDynamicVertexBuffer(char** pData) {
    GDriver->SetTexture(0, Texture);

    memcpy((*pData), Primitives(0)->pBuffer->m_pVB->pVertices,
           Primitives(0)->pBuffer->m_pVB->nVertices
           * Primitives(0)->pBuffer->m_pVB->Protocol->Decl->GetStride());
    for (unsigned int k = 0; k < Primitives(0)->pBuffer->m_pVB->nVertices; ++k) {
        *((TVector3*) &((*pData)[k
                                 * Primitives(0)->pBuffer->m_pVB->Protocol->Decl->GetStride()])) = TM
                                         .TransformVector3(
                                             *((TVector3*) &((*pData)[k
                                                     * Primitives(0)->pBuffer->m_pVB->Protocol->Decl->GetStride()])));
    }
    *pData += Primitives(0)->pBuffer->m_pVB->nVertices
              * Primitives(0)->pBuffer->m_pVB->Protocol->Decl->GetStride();
    return Primitives(0)->pBuffer->m_pVB->nVertices;
}

unsigned int CUIButtonPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex) {
    for (unsigned int k = 0; k < GetNumIndices(); ++k) {
        TIndex16 tmpIndex;
        tmpIndex._1 = Primitives(0)->pBuffer->m_pIB->pIndices[k]._1
                      + *BaseIndex;
        tmpIndex._2 = Primitives(0)->pBuffer->m_pIB->pIndices[k]._2
                      + *BaseIndex;
        tmpIndex._3 = Primitives(0)->pBuffer->m_pIB->pIndices[k]._3
                      + *BaseIndex;
        (*pData)[k] = tmpIndex;
    }
    *BaseIndex += Primitives(0)->pBuffer->m_pVB->nVertices;
    *pData += GetNumIndices();

    return Primitives(0)->pBuffer->m_pVB->nVertices;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

CUIButtonComponent::CUIButtonComponent(int TexIndex, int x, int y, int width, int height)
    :
    Width(width),
    Height(height),
    PosX(x),
    PosY(y) {
    /*CUIButtonPrimitive* ButtonPrimitive = new CUIButtonPrimitive();
     Primitives.AddItem(ButtonPrimitive);
     INT WinSizeX = 800;
     INT WinSizeY = 600;
     for(unsigned int i=0;i<ButtonPrimitive->Primitives.Size();++i)
     {
     Primitives(i)->Translation = TVector3((PosX / (float)WinSizeX)*0.0f-1.0f, ((WinSizeY - PosY) / (float)WinSizeY)*1.0f-0.0f, 0.0f);
     Primitives(i)->TM = TMatrix(TVector3((PosX / (float)WinSizeX)*1.0f+0.0f, ((WinSizeY - PosY)/ (float)WinSizeY)*1.0f-1.0f, 0.0f),
     TQuaternion(), TVector3((Width/(float)WinSizeX), (Height/(float)WinSizeY), 0.0f));
     }*/
}

CUIButtonComponent::~CUIButtonComponent() {

}

void CUIButtonComponent::UpdateComponent() {
}
