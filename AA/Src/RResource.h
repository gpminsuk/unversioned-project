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

class RAnimationSequence
{
public:
	class RBoneSequence
	{
	public:
		RBoneSequence(RAnimationSequence* Seq)
		{
			AnimSequenceRef = Seq;
		}

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
		RAnimationSequence* AnimSequenceRef;

		TVector3 GetPosKey(unsigned int InFrame)
		{
			if(PosKeys.Size() < 2)
			{
				return TVector3(0,0,0);
			}
			if(InFrame == 0)
			{
				return PosKeys(0).Pos;
			}
			int Idx = -1;
			for(unsigned int i=0;i<PosKeys.Size();++i)
			{
				POSKEY& PosKey = PosKeys(i);
				if(PosKey.Time > InFrame)
				{
					Idx = i;
					break;
				}
			}
			if(Idx == 0)
			{
				//Idx = (int)PosKeys.Size() - 1;
				//float t = 1.0f - (InFrame + AnimSequenceRef->EndFrame*AnimSequenceRef->TickPerFrame - PosKeys(Idx).Time)/((AnimSequenceRef->EndFrame*AnimSequenceRef->TickPerFrame - PosKeys(Idx).Time) + PosKeys(0).Time);
				//return (PosKeys(Idx).Pos*t + PosKeys(0).Pos*(1.0f - t));
				return PosKeys(0).Pos;
			}
			else if(Idx == -1)
			{
				Idx = (int)PosKeys.Size() - 1;
				//float t = 1.0f - (InFrame - PosKeys(Idx).Time)/((AnimSequenceRef->EndFrame*AnimSequenceRef->TickPerFrame - PosKeys(Idx).Time) + PosKeys(0).Time);
				//return (PosKeys(Idx).Pos*t + PosKeys(0).Pos*(1.0f - t));
				return PosKeys(Idx).Pos;
			}
			float t = 1.0f - (InFrame - PosKeys(Idx-1).Time)/(PosKeys(Idx).Time - PosKeys(Idx-1).Time);
			return (PosKeys(Idx-1).Pos*t + PosKeys(Idx).Pos*(1.0f - t));
		}

		TQuaternion GetRotKey(unsigned int InFrame)
		{
			if(RotKeys.Size() < 2)
			{
				return TQuaternion();
			}
			if(InFrame == 0)
			{
				return RotKeys(0).Rot;
			}
			int Idx = -1;
			for(unsigned int i=0;i<RotKeys.Size();++i)
			{
				ROTKEY& RotKey = RotKeys(i);
				if(RotKey.Time > InFrame)
				{
					Idx = i;
					break;
				}
			}
			if(Idx == 0)
			{
				//Idx = (int)RotKeys.Size() - 1;
				//float t = (InFrame + AnimSequenceRef->EndFrame*AnimSequenceRef->TickPerFrame - RotKeys(Idx).Time)/((AnimSequenceRef->EndFrame*AnimSequenceRef->TickPerFrame - RotKeys(Idx).Time) + RotKeys(0).Time);
				//return TQuaternion::Slerp(RotKeys(Idx).Rot, RotKeys(0).Rot, t);
				return RotKeys(0).Rot;
			}
			else if(Idx == -1)
			{
				Idx = (int)RotKeys.Size() - 1;
				//float t = (InFrame - RotKeys(Idx).Time)/(AnimSequenceRef->EndFrame*AnimSequenceRef->TickPerFrame - RotKeys(Idx).Time);
				//return TQuaternion::Slerp(RotKeys(Idx).Rot, RotKeys(0).Rot, t);
				return RotKeys(Idx).Rot;
			}
			float t = (InFrame - RotKeys(Idx-1).Time)/(RotKeys(Idx).Time - RotKeys(Idx-1).Time);
			return TQuaternion::Slerp(RotKeys(Idx-1).Rot, RotKeys(Idx).Rot, t);
		}
	};

	~RAnimationSequence()
	{
		for(unsigned int i = 0;i<AnimationBoneSequences.Size();++i)
			delete AnimationBoneSequences(i);
		AnimationBoneSequences.Clear(true);
	}

	TArray<RBoneSequence*> AnimationBoneSequences;

	unsigned int StartFrame;
	unsigned int EndFrame;
	unsigned int TickPerFrame;
	unsigned int FrameSpeed;
};
typedef RAnimationSequence::RBoneSequence RAnimationBoneSequence;

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

		TMatrix TM;
		TMatrix InvTM;
	};

	TMatrix GetBoneMatrix(RBone* Bone)
	{
		TMatrix Ret;
		Ret.SetIdentity();
		while(Bone)
		{
			Ret = Ret * Bone->TM;
			Bone = Bone->Parent;
		}
		return Ret;
	}

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