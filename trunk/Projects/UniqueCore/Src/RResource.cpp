#include "StdAfx.h"
#include "RResource.h"

TArray<RShaderBase*> RShaderTable::Shaders;

RVertexDeclaration* RVertexDeclaration::Position_Normal_TexCoord;
RVertexDeclaration* RVertexDeclaration::Position_Normal;
RVertexDeclaration* RVertexDeclaration::Position_TexCoord;
RVertexDeclaration* RVertexDeclaration::SkeletalMesh_GPU_Skin;
