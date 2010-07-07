#pragma once
#include "BPrimitive.h"

struct TParticleInstance
{
	TVector3 Position;
	TVector2 Size;
	float LifeTime;
	TVector3 Velocity;
	bool isAlive;
};

class TParticlePrimitive : public TPrimitive
{
public:
	TParticlePrimitive();
	virtual ~TParticlePrimitive();
};

class CParticlePrimitive : public BPrimitive
{
public:
	CParticlePrimitive(void);
	~CParticlePrimitive(void);

	void CreateParticlePrimitive(unsigned int ParticleCount);
	void DestroyParticlePrimitive();
	void UpdateParticlePrimitive(TArray<TParticleInstance*> DataSet, TVector3 CameraPosition);

	virtual void Render(BRenderingBatch *Batch);
	virtual unsigned int FillDynamicVertexBuffer(char** pData);
	virtual unsigned int FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex);
	virtual unsigned int GetNumIndices();

	unsigned int ParticlePoolSize;
	unsigned int CurrentParticleCount;
};
