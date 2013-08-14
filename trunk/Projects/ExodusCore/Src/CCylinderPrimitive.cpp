#include "StdAfx.h"
#include "CCylinderPrimitive.h"
#include "BRenderingBatch.h"

#include "RResource.h"

TCylinderPrimitive* GCylinderPrimitive;
TCylinderPrimitive* GCylinderPrimitiveWireFrame;

CCylinderPrimitive::CCylinderPrimitive(ERenderType _RenderType) {
    RenderType = _RenderType;
    switch (RenderType) {
    case RenderType_Opaque:
    case RenderType_Translucent:
        Draws.AddItem(GCylinderPrimitive);
        break;
    case RenderType_Line:
        Draws.AddItem(GCylinderPrimitiveWireFrame);
        break;
    }
}

CCylinderPrimitive::~CCylinderPrimitive(void) {
}

TCylinderPrimitive::TCylinderPrimitive(ERenderType _RenderType) {
    switch (_RenderType) {
    case RenderType_Line: {
        pBuffer = new RStaticPrimitiveBuffer();

        RSystemMemoryVertexBuffer *pVB = new RSystemMemoryVertexBuffer();
        pBuffer->m_pVB = pVB;
        pBuffer->m_pIB = 0;

        struct VD {
            TVector3 Pos;
        };

        int Smoothness = 24;

        pVB->nVertices = Smoothness * 6;
        pVB->pVertices = new char[sizeof(VD) * pVB->nVertices];

        VD *Vertex = reinterpret_cast<VD*>(pVB->pVertices);

        float Theta = 0.0f;
        for (unsigned int i = 0; i < pVB->nVertices / 6; ++i) {
            Vertex[i * 6 + 0].Pos = TVector3(COSINE(Theta), 1.0f, SINE(Theta));
            Vertex[i * 6 + 1].Pos = TVector3(COSINE(Theta), 0.0f, SINE(Theta));
            Vertex[i * 6 + 2].Pos = TVector3(COSINE(Theta), 1.0f, SINE(Theta));
            Vertex[i * 6 + 4].Pos = TVector3(COSINE(Theta), 0.0f, SINE(Theta));
            Theta += (MATH_PI * 2.0f) / ((float) pVB->nVertices / 6.0f);
            Vertex[i * 6 + 3].Pos = TVector3(COSINE(Theta), 1.0f, SINE(Theta));
            Vertex[i * 6 + 5].Pos = TVector3(COSINE(Theta), 0.0f, SINE(Theta));
        }
    }
    break;
    case RenderType_Opaque:
    case RenderType_Translucent: {
        pBuffer = new RStaticPrimitiveBuffer();

        RSystemMemoryVertexBuffer *pVB = new RSystemMemoryVertexBuffer();
        RSystemMemoryIndexBuffer *pIB = new RSystemMemoryIndexBuffer();
        pBuffer->m_pVB = pVB;
        pBuffer->m_pIB = pIB;

        struct VD {
            TVector3 Pos;
            TVector3 Normal;
            TVector2 UV;
        };

        int Smoothness = 24;

        pVB->nVertices = Smoothness * 10;
        pVB->pVertices = new char[sizeof(VD) * pVB->nVertices];

        VD *Vertex = reinterpret_cast<VD*>(pVB->pVertices);

        float Theta = 0.0f;
        for (unsigned int i = 0; i < pVB->nVertices / 10; ++i) {
            float NextTheta = Theta
                              + (MATH_PI * 2.0f) / ((float) pVB->nVertices / 10.0f);
            Vertex[i * 10 + 0].Pos = TVector3(0.0f, 1.0f, 0.0f);
            Vertex[i * 10 + 0].Normal = TVector3(0.0f, 1.0f, 0.0f);
            Vertex[i * 10 + 1].Pos = TVector3(COSINE(Theta), 1.0f, SINE(Theta));
            Vertex[i * 10 + 1].Normal = TVector3(0.0f, 1.0f, 0.0f);
            Vertex[i * 10 + 2].Pos = TVector3(COSINE(NextTheta), 1.0f,
                                              SINE(NextTheta));
            Vertex[i * 10 + 2].Normal = TVector3(0.0f, 1.0f, 0.0f);

            Vertex[i * 10 + 3].Pos = TVector3(COSINE(Theta), 1.0f, SINE(Theta));
            Vertex[i * 10 + 3].Normal = TVector3(COSINE(Theta), 0.0f,
                                                 SINE(Theta));
            Vertex[i * 10 + 4].Pos = TVector3(COSINE(NextTheta), 1.0f,
                                              SINE(NextTheta));
            Vertex[i * 10 + 4].Normal = TVector3(COSINE(NextTheta), 0.0f,
                                                 SINE(NextTheta));
            Vertex[i * 10 + 5].Pos = TVector3(COSINE(NextTheta), 0.0f,
                                              SINE(NextTheta));
            Vertex[i * 10 + 5].Normal = TVector3(COSINE(NextTheta), 0.0f,
                                                 SINE(NextTheta));
            Vertex[i * 10 + 6].Pos = TVector3(COSINE(Theta), 0.0f, SINE(Theta));
            Vertex[i * 10 + 6].Normal = TVector3(COSINE(Theta), 0.0f,
                                                 SINE(Theta));

            Vertex[i * 10 + 7].Pos = TVector3(0.0f, 0.0f, 0.0f);
            Vertex[i * 10 + 7].Normal = TVector3(0.0f, -1.0f, 0.0f);
            Vertex[i * 10 + 8].Pos = TVector3(COSINE(Theta), 0.0f, SINE(Theta));
            Vertex[i * 10 + 8].Normal = TVector3(0.0f, -1.0f, 0.0f);
            Vertex[i * 10 + 9].Pos = TVector3(COSINE(NextTheta), 0.0f,
                                              SINE(NextTheta));
            Vertex[i * 10 + 9].Normal = TVector3(0.0f, -1.0f, 0.0f);
            Theta = NextTheta;
        }

        pIB->nIndices = Smoothness * 4;
        pIB->pIndices = new TIndex16[pIB->nIndices];

        for (unsigned int i = 0; i < pIB->nIndices / 4; ++i) {
            pIB->pIndices[i * 4 + 0] = TIndex16(i * 10 + 0, i * 10 + 2,
                                                i * 10 + 1);
            pIB->pIndices[i * 4 + 1] = TIndex16(i * 10 + 3, i * 10 + 4,
                                                i * 10 + 5);
            pIB->pIndices[i * 4 + 2] = TIndex16(i * 10 + 3, i * 10 + 5,
                                                i * 10 + 6);
            pIB->pIndices[i * 4 + 3] = TIndex16(i * 10 + 7, i * 10 + 8,
                                                i * 10 + 9);
        }
    }
    break;
    }
}

unsigned int CCylinderPrimitive::GetNumIndices() {
    return Draws(0)->pBuffer->m_pIB->nIndices;
}

unsigned int CCylinderPrimitive::FillDynamicVertexBuffer(char** pData) {
    memcpy((*pData), Draws(0)->pBuffer->m_pVB->pVertices,
           Draws(0)->pBuffer->m_pVB->nVertices
           * Draws(0)->pBuffer->m_pVB->Protocol->Decl->GetStride());
    for (unsigned int k = 0; k < Draws(0)->pBuffer->m_pVB->nVertices; ++k) {
        *((TVector3*) &((*pData)[k
                                 * Draws(0)->pBuffer->m_pVB->Protocol->Decl->GetStride()])) = TM
                                         .TransformVector3(
                                             *((TVector3*) &((*pData)[k
                                                     * Draws(0)->pBuffer->m_pVB->Protocol->Decl->GetStride()])));
    }
    *pData += Draws(0)->pBuffer->m_pVB->nVertices
              * Draws(0)->pBuffer->m_pVB->Protocol->Decl->GetStride();
    return Draws(0)->pBuffer->m_pVB->nVertices;
}

unsigned int CCylinderPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex) {
    for (unsigned int k = 0; k < GetNumIndices(); ++k) {
        TIndex16 tmpIndex;
        tmpIndex._1 = Draws(0)->pBuffer->m_pIB->pIndices[k]._1
                      + *BaseIndex;
        tmpIndex._2 = Draws(0)->pBuffer->m_pIB->pIndices[k]._2
                      + *BaseIndex;
        tmpIndex._3 = Draws(0)->pBuffer->m_pIB->pIndices[k]._3
                      + *BaseIndex;
        (*pData)[k] = tmpIndex;
    }
    *BaseIndex += Draws(0)->pBuffer->m_pVB->nVertices;
    *pData += GetNumIndices();

    return Draws(0)->pBuffer->m_pVB->nVertices;
}
