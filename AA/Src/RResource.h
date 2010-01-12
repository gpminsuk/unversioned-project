#pragma once

// Shader
// IndexBuffer
// VertexBuffer
// Texture

#include "TDataTypes.h"

class BViewport;

class RShaderBase
{
public:
	RShaderBase() {}
	virtual ~RShaderBase() {}

	virtual bool BeginShader() = 0;
	virtual bool SetParameter(BViewport* vp) = 0;
	virtual bool EndShader() = 0;

	wchar_t m_FileName[256];
protected:
};

class RShaderTable
{
public:
	static TArray<RShaderBase*> Shaders;
};

class RSystemMemoryIndexBuffer
{
public:
	RSystemMemoryIndexBuffer() : pIndices(0), nIndices(0) {}
	virtual ~RSystemMemoryIndexBuffer()
	{
		delete[] pIndices; pIndices = 0;
		nIndices = 0;
	}

	TIndex16 *pIndices;
	unsigned int nIndices;
};

class RVideoMemoryIndexBuffer
{
public:
	RVideoMemoryIndexBuffer() : nIndices(0) {}
	virtual ~RVideoMemoryIndexBuffer()
	{
	   nIndices = 0;
	}
	
	unsigned int nIndices;
};

class RSystemMemoryIndexBufferTable
{
public:
	static TArray<RSystemMemoryIndexBuffer*> IndexBuffers;
};

// primitive per one DP

enum VD_TYPE
{
	DECLTYPE_FLOAT1 = 0,
	DECLTYPE_FLOAT2 = 1,
	DECLTYPE_FLOAT3 = 2,
	DECLTYPE_FLOAT4 = 3,
	DECLTYPE_COLOR = 4,
	DECLTYPE_UBYTE4 = 5,  
	DECLTYPE_SHORT2 = 6,
	DECLTYPE_SHORT4 = 7,
	DECLTYPE_UBYTE4N = 8,
	DECLTYPE_SHORT2N = 9,
	DECLTYPE_SHORT4N = 10,
	DECLTYPE_USHORT2N = 11,
	DECLTYPE_USHORT4N = 12,
	DECLTYPE_UDEC3 = 13,
	DECLTYPE_DEC3N = 14,
	DECLTYPE_FLOAT16_2 = 15,
	DECLTYPE_FLOAT16_4 = 16,
	DECLTYPE_UNUSED = 17,
};

struct VertexDeclaration
{
	short Offset;
	char Type;
};

class RSystemMemoryVertexBuffer
{
public:
	RSystemMemoryVertexBuffer() : pVertices(0), nVertices(0), Declaration(0) {}
	virtual ~RSystemMemoryVertexBuffer()
	{
		delete[] pVertices; pVertices = 0;
		delete[] Declaration; Declaration = 0;
		nVertices = 0;
	}

	unsigned int nVertices;
	unsigned int nVertexStride;

	VertexDeclaration* Declaration;

	char *pVertices;
};

class RVideoMemoryVertexBuffer
{
public:
	RVideoMemoryVertexBuffer() : nVertices(0), Declaration(0) {}
	virtual ~RVideoMemoryVertexBuffer()
	{
		delete[] Declaration; Declaration = 0;
		nVertices = 0;
	}

	unsigned int nVertices;
	unsigned int nVertexStride;

	VertexDeclaration* Declaration;
};

class RSystemMemoryVertexBufferTable
{
public:
	static TArray<RSystemMemoryVertexBuffer*> VertexBuffers;
};

class RPrimitiveBuffer
{
public:
};

class RDynamicPrimitiveBuffer : public RPrimitiveBuffer
{
public:
	RDynamicPrimitiveBuffer() : m_pVB(0), m_pIB(0) {}
	virtual ~RDynamicPrimitiveBuffer() {}

	RVideoMemoryVertexBuffer* m_pVB;
	RVideoMemoryIndexBuffer* m_pIB;

	virtual void Release() = 0;
};

class RStaticPrimitiveBuffer : public RPrimitiveBuffer
{
public:
	RSystemMemoryVertexBuffer* m_pVB;
	RSystemMemoryIndexBuffer* m_pIB;
};

class RTexture
{
protected:
	enum ETextureMemoryType
	{
		TMT_SYSTEM,
		TMT_MANAGED,
		TMT_VIDEO
	};
	int Width;
	int Height;
	ETextureMemoryType MemoryType;
};

class RTextureTable
{
public:
	static TArray<RTexture*> Textures;
};

class RTextureBuffer
{
public:
	RTextureBuffer() {}
	virtual ~RTextureBuffer() {}

	unsigned int Width;
	unsigned int Height;

	virtual struct TLockedRect Lock() = 0;
	virtual bool Unlock() = 0;
};

class RTextureBufferTable
{
public:
	static TArray<RTextureBuffer*> TextureBuffers;
};

class RMaterial
{
public:
	int TID;
};

class RRenderTarget
{
public:
	RRenderTarget() : m_pTexture(0) {}
	virtual ~RRenderTarget() {}

	RTextureBuffer* m_pTexture;

	unsigned int m_SizeX;
	unsigned int m_SizeY;

	virtual bool Release() = 0;
};

class RAnimationBoneSequence
{
public:
	TString				BoneName;

	struct POSKEY
	{
		TVector3 Pos;
		float Time;
	};

	struct ROTKEY
	{
		TQuaternion Rot;
		float Time;
	};

	TArray<POSKEY>	PosKeys;
	TArray<ROTKEY>	RotKeys;
};

class RAnimationSequence
{
public:
	~RAnimationSequence()
	{
		for(unsigned int i = 0;i<AnimationBoneSequences.Size();++i)
			delete AnimationBoneSequences(i);
		AnimationBoneSequences.Clear(true);
	}

	TArray<RAnimationBoneSequence*> AnimationBoneSequences;

	unsigned int StartFrame;
	unsigned int EndFrame;
	unsigned int TickPerFrame;
	unsigned int FrameSpeed;
};

class RAnimationSequenceTable
{
public:
	static TArray<RAnimationSequence*> Sequences;
};

class RSkeletalSubMesh
{
public:
	typedef struct
	{
		TVector3 Pos;
		TVector3 Normal;
		TVector2 UV;
	} VD;
	typedef TIndex16 ID;

	TString				BoneName;

	RSystemMemoryVertexBuffer*	pVB;
	RSystemMemoryIndexBuffer* pIB;
};

class RSkeletalMesh
{
public:
	~RSkeletalMesh()
	{
		for(unsigned int i = 0;i<SkeletalSubMeshes.Size();++i)
			delete SkeletalSubMeshes(i);
		SkeletalSubMeshes.Clear(true);
	}
	TArray<RSkeletalSubMesh*>	SkeletalSubMeshes;
};

class RSkeletalMeshTable
{
public:
	static TArray<RSkeletalMesh*> SkeletalMeshes;
};

class RBoneHierarchy
{
public:
	class RBone
	{
	public:
		RBone() : Parent(0) {}
		~RBone()
		{
			for(unsigned int i=0;i<ChildBones.Size();++i)
			{
				delete ChildBones(i);
			}
		}

		void AddBone_Recursive(RBone* Bone, TString ParentName)
		{
			if(BoneName == ParentName)
			{
				ChildBones.AddItem(Bone);
				Bone->Parent = this;
			}
			else
			{
				for(unsigned int i=0;i<ChildBones.Size();++i)
				{
					ChildBones(i)->AddBone_Recursive(Bone, ParentName);
				}
			}			
		}

		TString BoneName;
		RBone *Parent;
		TArray<RBone*> ChildBones;

		TMatrix BoneTM;
	};

	void AddBone(RBone* Bone, TString ParentName)
	{
		if(ParentName == "")
		{
			RootBone = Bone;
		}
		else
		{
			RootBone->AddBone_Recursive(Bone, ParentName);
		}
	}

	RBoneHierarchy()
		: RootBone(0)
	{
	}

	~RBoneHierarchy()
	{
		delete RootBone;
	}

	RBone *RootBone;
};

class RBoneHierarchyTable
{
public:
	static TArray<RBoneHierarchy*> BoneHierarchies;
};