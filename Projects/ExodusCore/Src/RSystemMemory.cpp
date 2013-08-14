#include "stdafx.h"
#include "RMesh.h"
#include "BDriver.h"

IMPLEMENT_CLASS(RSystemMemoryIndexBuffer);
IMPLEMENT_CLASS(RSystemMemoryVertexBuffer);

bool RSystemMemoryIndexBuffer::Access(AAccessor& Accessor) {
    Accessor << nIndices;
    if (Accessor.IsLoading()) {
        pIndices = new TIndex16[nIndices];
    }
    for (unsigned int i = 0; i < nIndices; ++i) {
        Accessor << pIndices[i];
    }
    return true;
}

bool RSystemMemoryVertexBuffer::Access(AAccessor& Accessor) {
	unsigned int ProtocolIndex = 1;
    Accessor << ProtocolIndex;
    Accessor << nVertices;
    if (Accessor.IsLoading()) {
		Protocol = RVertexProtocol::Protocols(ProtocolIndex);
        pVertices = new char[sizeof(RVertexDeclaration::SkeletalMesh_GPU_Skin_VD) * nVertices];
    }
    for (unsigned int i = 0; i < nVertices; ++i) {
        Accessor.Access(&pVertices[i * sizeof(RVertexDeclaration::SkeletalMesh_GPU_Skin_VD)], sizeof(RVertexDeclaration::SkeletalMesh_GPU_Skin_VD));
    }
    return true;
}