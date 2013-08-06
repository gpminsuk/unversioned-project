#include "StdAfx.h"

#include "RResource.h"
#include "BRenderingBatch.h"

#include "CParticlePrimitive.h"

TParticlePrimitive::TParticlePrimitive() {
}

TParticlePrimitive::~TParticlePrimitive() {
}

CParticlePrimitive::CParticlePrimitive()
    :
    CurrentParticleCount(0),
    ParticlePoolSize(0)

{
    RenderType = RenderType_Particle;
}

CParticlePrimitive::~CParticlePrimitive() {
    DestroyParticlePrimitive();
}

void CParticlePrimitive::CreateParticlePrimitive(unsigned int ParticleCount) {
    DestroyParticlePrimitive();

    ParticlePoolSize = ParticleCount;

    TParticlePrimitive *Primitive = new TParticlePrimitive();
    Primitives.AddItem(Primitive);

    Primitive->pBuffer = new RStaticPrimitiveBuffer();

    RSystemMemoryVertexBuffer *pVB = new RSystemMemoryVertexBuffer();
    Primitive->pBuffer->m_pVB = pVB;

    RSystemMemoryIndexBuffer *pIB = new RSystemMemoryIndexBuffer();
    Primitive->pBuffer->m_pIB = pIB;

	pVB->Declaration = RVertexDeclaration::Position_TexCoord;
    pVB->nVertices = ParticleCount * 4;
    pVB->pVertices = new char[pVB->Declaration->GetStride() * pVB->nVertices];

    RVertexDeclaration::Position_TexCoord_VD *Vertex = reinterpret_cast<RVertexDeclaration::Position_TexCoord_VD*>(pVB->pVertices);
	
    for (unsigned int i = 0; i < ParticleCount; ++i) {
        Vertex[i * 4 + 0].Position = TVector3(-1, -1, 0);
        Vertex[i * 4 + 0].TexCoord = TVector2(0, 0);

        Vertex[i * 4 + 1].Position = TVector3(-1, 1, 0);
        Vertex[i * 4 + 1].TexCoord = TVector2(0, 1);

        Vertex[i * 4 + 2].Position = TVector3(1, -1, 0);
        Vertex[i * 4 + 2].TexCoord = TVector2(1, 0);

        Vertex[i * 4 + 3].Position = TVector3(1, 1, 0);
        Vertex[i * 4 + 3].TexCoord = TVector2(1, 1);
    }

    pIB->nIndices = ParticleCount * 2;
    pIB->pIndices = new TIndex16[pIB->nIndices];

    for (unsigned int i = 0; i < ParticleCount; ++i) {
        pIB->pIndices[i * 2 + 0] = TIndex16(i * 4 + 0, i * 4 + 1, i * 4 + 2);
        pIB->pIndices[i * 2 + 1] = TIndex16(i * 4 + 1, i * 4 + 3, i * 4 + 2);
    }
}

void CParticlePrimitive::UpdateParticlePrimitive(TArray<TParticleInstance*> DataSet, TVector3 CameraPosition) {
    int TempCurrentParticleCount = 0;
    for (unsigned int i = 0; i < DataSet.Size(); ++i) {
        TParticleInstance* Inst = DataSet(i);
        if (Inst->isAlive) {
            TVector3 RotateVector =
                (CameraPosition - Inst->Position).Normalize();
            float k = -RotateVector.x / RotateVector.z;
            TVector3 TangentVector(1, 0, k);
            TangentVector.Normalize();
            k = -RotateVector.y / RotateVector.x;
            TVector3 TangentVector2(k, 1, 0);
            TangentVector2.Normalize();
            //float Theta = ARCCOSINE(RotateVector.x/(ARCCOSINE(RotateVector.y)));
            //float Pi = ARCSINE(RotateDirection.y);

            RVertexDeclaration::Position_TexCoord_VD *Vertex = reinterpret_cast<RVertexDeclaration::Position_TexCoord_VD*>(Primitives(0)->pBuffer->m_pVB
                                               ->pVertices);
            TIndex16 *Index = reinterpret_cast<TIndex16*>(Primitives(0)->pBuffer
                              ->m_pIB->pIndices);
            if (i
                    < (unsigned int) (Primitives(0)->pBuffer->m_pVB->nVertices
                                      / 4)) {
                Vertex[i * 4 + 0].Position = Inst->Position + -TangentVector
                                        + -TangentVector2;
                Vertex[i * 4 + 1].Position = Inst->Position + -TangentVector
                                        + TangentVector2;
                Vertex[i * 4 + 2].Position = Inst->Position + TangentVector
                                        + -TangentVector2;
                Vertex[i * 4 + 3].Position = Inst->Position + TangentVector
                                        + TangentVector2;

                /*Vertex[i*4+0].Pos = Inst->Position + TVector3(RotateVector.y,-RotateVector.x,-1);
                 Vertex[i*4+1].Pos = Inst->Position + TVector3(RotateVector.y,-RotateVector.x,1);
                 Vertex[i*4+2].Pos = Inst->Position + TVector3(-RotateVector.y,RotateVector.x,-1);
                 Vertex[i*4+3].Pos = Inst->Position + TVector3(-RotateVector.y,RotateVector.x,1);*/

                Index[TempCurrentParticleCount * 2 + 0] = TIndex16(i * 4 + 0,
                        i * 4 + 2, i * 4 + 1);
                Index[TempCurrentParticleCount * 2 + 1] = TIndex16(i * 4 + 1,
                        i * 4 + 2, i * 4 + 3);
                ++TempCurrentParticleCount;
            }
        }
    }
    CurrentParticleCount = TempCurrentParticleCount;
}

void CParticlePrimitive::DestroyParticlePrimitive() {
    for (unsigned int i = 0; i < Primitives.Size(); ++i) {
        delete Primitives(i);
    }
    Primitives.Clear();
}

unsigned int CParticlePrimitive::GetNumIndices() {
    return CurrentParticleCount * 2;
}

unsigned int CParticlePrimitive::FillDynamicVertexBuffer(char** pData) {
    memcpy((*pData), Primitives(0)->pBuffer->m_pVB->pVertices,
           Primitives(0)->pBuffer->m_pVB->nVertices
           * Primitives(0)->pBuffer->m_pVB->Declaration->GetStride());
    for (unsigned int k = 0; k < Primitives(0)->pBuffer->m_pVB->nVertices; ++k) {
        (*((RVertexDeclaration::Position_TexCoord_VD*) &((*pData)[k * Primitives(0)->pBuffer->m_pVB->Declaration->GetStride()])))
        .Position = TM.TransformVector3(
                   *((TVector3*) &((*pData)[k
                                            * Primitives(0)->pBuffer->m_pVB->Declaration->GetStride()])));
        //(*((VD*)&((*pData)[k*Primitives(0)->pBuffer->m_pVB->nVertexStride]))).Size = TVector3(0.1f,0.1f,0.1f);
    }
    *pData += Primitives(0)->pBuffer->m_pVB->nVertices
              * Primitives(0)->pBuffer->m_pVB->Declaration->GetStride();
    return Primitives(0)->pBuffer->m_pVB->nVertices;
}

unsigned int CParticlePrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex) {
    unsigned int nIndices = GetNumIndices();
    for (unsigned int k = 0; k < nIndices; ++k) {
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
    *pData += nIndices;

    return Primitives(0)->pBuffer->m_pVB->nVertices;
}
