#include "StdAfx.h"
#include "CBoxPrimitive.h"
#include "BRenderingBatch.h"

#include "RResource.h"

TBoxPrimitive* GBoxPrimitiveOutside;
TBoxPrimitive* GBoxPrimitiveInside;

CBoxPrimitive::CBoxPrimitive(ERenderType _RenderType, EGeometrySideType _BoxSideType) {
    RenderType = _RenderType;
    SideType = _BoxSideType;

    switch (_RenderType) {
    case RenderType_Opaque: {
        switch (_BoxSideType) {
        case SideType_Inside:
            Primitives.AddItem(GBoxPrimitiveInside);
            break;
        case SideType_Outside:
            Primitives.AddItem(GBoxPrimitiveOutside);
            break;
        }
    }
    break;
    }
}

CBoxPrimitive::~CBoxPrimitive(void) {
}

TBoxPrimitive::TBoxPrimitive(ERenderType _RenderType, EGeometrySideType _BoxSideType) {
    switch (_RenderType) {
    case RenderType_Opaque: {
        pBuffer = new RStaticPrimitiveBuffer();

        RSystemMemoryVertexBuffer *pVB = new RSystemMemoryVertexBuffer();
        RSystemMemoryIndexBuffer *pIB = new RSystemMemoryIndexBuffer();
        pBuffer->m_pVB = pVB;
        pBuffer->m_pIB = pIB;
				
		pVB->Declaration = RVertexDeclaration::Position_Normal;
        pVB->nVertices = 24;
        pVB->pVertices = new char[pVB->Declaration->GetStride() * pVB->nVertices];

        RVertexDeclaration::Position_Normal_VD *Vertex = reinterpret_cast<RVertexDeclaration::Position_Normal_VD*>(pVB->pVertices);

        Vertex[0].Position = TVector3(-1.0f, -1.0f, -1.0f);
        Vertex[1].Position = TVector3(1.0f, -1.0f, -1.0f);
        Vertex[2].Position = TVector3(-1.0f, 1.0f, -1.0f);
        Vertex[3].Position = TVector3(1.0f, 1.0f, -1.0f);

        Vertex[4].Position = TVector3(1.0f, -1.0f, 1.0f);
        Vertex[5].Position = TVector3(-1.0f, -1.0f, 1.0f);
        Vertex[6].Position = TVector3(1.0f, 1.0f, 1.0f);
        Vertex[7].Position = TVector3(-1.0f, 1.0f, 1.0f);

        Vertex[8].Position = TVector3(1.0f, -1.0f, -1.0f);
        Vertex[9].Position = TVector3(-1.0f, -1.0f, -1.0f);
        Vertex[10].Position = TVector3(1.0f, -1.0f, 1.0f);
        Vertex[11].Position = TVector3(-1.0f, -1.0f, 1.0f);

        Vertex[12].Position = TVector3(-1.0f, 1.0f, -1.0f);
        Vertex[13].Position = TVector3(1.0f, 1.0f, -1.0f);
        Vertex[14].Position = TVector3(-1.0f, 1.0f, 1.0f);
        Vertex[15].Position = TVector3(1.0f, 1.0f, 1.0f);

        Vertex[16].Position = TVector3(-1.0f, -1.0f, -1.0f);
        Vertex[17].Position = TVector3(-1.0f, 1.0f, -1.0f);
        Vertex[18].Position = TVector3(-1.0f, -1.0f, 1.0f);
        Vertex[19].Position = TVector3(-1.0f, 1.0f, 1.0f);

        Vertex[20].Position = TVector3(1.0f, 1.0f, -1.0f);
        Vertex[21].Position = TVector3(1.0f, -1.0f, -1.0f);
        Vertex[22].Position = TVector3(1.0f, 1.0f, 1.0f);
        Vertex[23].Position = TVector3(1.0f, -1.0f, 1.0f);

        pIB->nIndices = 12;
        pIB->pIndices = new TIndex16[pIB->nIndices];
        switch (_BoxSideType) {
        case SideType_Outside: {
            Vertex[0].Normal = TVector3(0.0f, 0.0f, -1.0f);
            Vertex[1].Normal = TVector3(0.0f, 0.0f, -1.0f);
            Vertex[2].Normal = TVector3(0.0f, 0.0f, -1.0f);
            Vertex[3].Normal = TVector3(0.0f, 0.0f, -1.0f);

            Vertex[4].Normal = TVector3(0.0f, 0.0f, 1.0f);
            Vertex[5].Normal = TVector3(0.0f, 0.0f, 1.0f);
            Vertex[6].Normal = TVector3(0.0f, 0.0f, 1.0f);
            Vertex[7].Normal = TVector3(0.0f, 0.0f, 1.0f);

            Vertex[8].Normal = TVector3(0.0f, -1.0f, 0.0f);
            Vertex[9].Normal = TVector3(0.0f, -1.0f, 0.0f);
            Vertex[10].Normal = TVector3(0.0f, -1.0f, 0.0f);
            Vertex[11].Normal = TVector3(0.0f, -1.0f, 0.0f);

            Vertex[12].Normal = TVector3(0.0f, 1.0f, 0.0f);
            Vertex[13].Normal = TVector3(0.0f, 1.0f, 0.0f);
            Vertex[14].Normal = TVector3(0.0f, 1.0f, 0.0f);
            Vertex[15].Normal = TVector3(0.0f, 1.0f, 0.0f);

            Vertex[16].Normal = TVector3(-1.0f, 0.0f, 0.0f);
            Vertex[17].Normal = TVector3(-1.0f, 0.0f, 0.0f);
            Vertex[18].Normal = TVector3(-1.0f, 0.0f, 0.0f);
            Vertex[19].Normal = TVector3(-1.0f, 0.0f, 0.0f);

            Vertex[20].Normal = TVector3(1.0f, 0.0f, 0.0f);
            Vertex[21].Normal = TVector3(1.0f, 0.0f, 0.0f);
            Vertex[22].Normal = TVector3(1.0f, 0.0f, 0.0f);
            Vertex[23].Normal = TVector3(1.0f, 0.0f, 0.0f);

            for (int i = 0; i < 6; ++i) {
                pIB->pIndices[i * 2 + 0] = TIndex16(i * 4 + 0, i * 4 + 2,
                                                    i * 4 + 1);
                pIB->pIndices[i * 2 + 1] = TIndex16(i * 4 + 2, i * 4 + 3,
                                                    i * 4 + 1);
            }
        }
        break;
        case SideType_Inside: {
            Vertex[0].Normal = TVector3(0.0f, 0.0f, 1.0f);
            Vertex[1].Normal = TVector3(0.0f, 0.0f, 1.0f);
            Vertex[2].Normal = TVector3(0.0f, 0.0f, 1.0f);
            Vertex[3].Normal = TVector3(0.0f, 0.0f, 1.0f);

            Vertex[4].Normal = TVector3(0.0f, 0.0f, -1.0f);
            Vertex[5].Normal = TVector3(0.0f, 0.0f, -1.0f);
            Vertex[6].Normal = TVector3(0.0f, 0.0f, -1.0f);
            Vertex[7].Normal = TVector3(0.0f, 0.0f, -1.0f);

            Vertex[8].Normal = TVector3(0.0f, 1.0f, 0.0f);
            Vertex[9].Normal = TVector3(0.0f, 1.0f, 0.0f);
            Vertex[10].Normal = TVector3(0.0f, 1.0f, 0.0f);
            Vertex[11].Normal = TVector3(0.0f, 1.0f, 0.0f);

            Vertex[12].Normal = TVector3(0.0f, -1.0f, 0.0f);
            Vertex[13].Normal = TVector3(0.0f, -1.0f, 0.0f);
            Vertex[14].Normal = TVector3(0.0f, -1.0f, 0.0f);
            Vertex[15].Normal = TVector3(0.0f, -1.0f, 0.0f);

            Vertex[16].Normal = TVector3(1.0f, 0.0f, 0.0f);
            Vertex[17].Normal = TVector3(1.0f, 0.0f, 0.0f);
            Vertex[18].Normal = TVector3(1.0f, 0.0f, 0.0f);
            Vertex[19].Normal = TVector3(1.0f, 0.0f, 0.0f);

            Vertex[20].Normal = TVector3(-1.0f, 0.0f, 0.0f);
            Vertex[21].Normal = TVector3(-1.0f, 0.0f, 0.0f);
            Vertex[22].Normal = TVector3(-1.0f, 0.0f, 0.0f);
            Vertex[23].Normal = TVector3(-1.0f, 0.0f, 0.0f);

            for (int i = 0; i < 6; ++i) {
                pIB->pIndices[i * 2 + 0] = TIndex16(i * 4 + 0, i * 4 + 1,
                                                    i * 4 + 2);
                pIB->pIndices[i * 2 + 1] = TIndex16(i * 4 + 2, i * 4 + 1,
                                                    i * 4 + 3);
            }
        }
        break;
        }
    }
    break;
    }
}

unsigned int CBoxPrimitive::GetNumIndices() {
    return Primitives(0)->pBuffer->m_pIB->nIndices;
}

RShaderBase* CBoxPrimitive::GetShaderType() {
	return RShaderTable::Shaders(0);
}

unsigned int CBoxPrimitive::FillDynamicVertexBuffer(char** pData) {
    memcpy((*pData), Primitives(0)->pBuffer->m_pVB->pVertices,
           Primitives(0)->pBuffer->m_pVB->nVertices
           * Primitives(0)->pBuffer->m_pVB->Declaration->GetStride());
    for (unsigned int k = 0; k < Primitives(0)->pBuffer->m_pVB->nVertices; ++k) {
        *((TVector3*) &((*pData)[k
                                 * Primitives(0)->pBuffer->m_pVB->Declaration->GetStride()])) = TM
                                         .TransformVector3(
                                             *((TVector3*) &((*pData)[k
                                                     * Primitives(0)->pBuffer->m_pVB->Declaration->GetStride()])));
    }
    *pData += Primitives(0)->pBuffer->m_pVB->nVertices
              * Primitives(0)->pBuffer->m_pVB->Declaration->GetStride();
    return Primitives(0)->pBuffer->m_pVB->nVertices;
}

unsigned int CBoxPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex) {
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
