#include "StdAfx.h"
#include "RResource.h"

TArray<RSystemMemoryIndexBuffer*>		RSystemMemoryIndexBufferTable::IndexBuffers;
TArray<RSystemMemoryVertexBuffer*>		RSystemMemoryVertexBufferTable::VertexBuffers;
TArray<RTexture*>						RTextureTable::Textures;
TArray<RShaderBase*>					RShaderTable::Shaders;
TArray<RTextureBuffer*>					RTextureBufferTable::TextureBuffers;
TArray<RAnimationSequence*>				RAnimationSequenceTable::Sequences;
TArray<RSkeletalMesh*>					RSkeletalMeshTable::SkeletalMeshes;
TArray<RBoneHierarchy*>					RBoneHierarchyTable::BoneHierarchies;