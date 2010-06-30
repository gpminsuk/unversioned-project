#include "StdAfx.h"
#include "CEmitter.h"

#include "CParticlePrimitive.h"

#include "UWorld.h"
#include "BCamera.h"

CEmitter::CEmitter()
:	ParticleIndex(0),
	CurrentParticleNum(0)
{
	for(int i=0;i<100;++i)
	{
		TParticleInstance* Inst = new TParticleInstance();
		Inst->isAlive = false;
		Inst->Position = TVector3(0,0,0);
		Inst->Size = TVector2(0.05f,0.5f);
		Particles.AddItem(Inst);
	}
	//Primitive->CreateParticlePrimitive(100);
}

CEmitter::~CEmitter()
{
	for(unsigned int i=0;i<Particles.Size();++i)
	{
		delete Particles(i);
	}
}

void CEmitter::Tick(unsigned long dTime)
{
	/*for(unsigned int i=0;i<Primitives.Size();++i)
	{
		CParticlePrimitive* Prim = dynamic_cast<CParticlePrimitive*>(Primitives(i));
		if(Prim)
		{
			Prim->UpdateParticlePrimitive(Particles, GWorld->m_pCamera->m_Location);
		}
	}

	static unsigned long TickCount = GetTickCount();
	if(GetTickCount() - TickCount > 1000)
	{
		TickCount = GetTickCount() - 1000000000;

		++ParticleIndex;
		if(ParticleIndex >= 100)
			ParticleIndex = 0;

		if(!Particles(ParticleIndex)->isAlive)
			++CurrentParticleNum;

		Particles(ParticleIndex)->LifeTime = 5;
		Particles(ParticleIndex)->Position = TVector3(0,0,0);
		Particles(ParticleIndex)->Size = TVector2(0.5f,0.5f);
		Particles(ParticleIndex)->Velocity = TVector3(0.0/*(rand()%10)/10.0f-0.5f*//*,0,0);
		Particles(ParticleIndex)->isAlive = true;
	}
	static unsigned long TC = GetTickCount();
	DWORD TickN = GetTickCount()-TC;	
	for(unsigned int i=0;i<Particles.Size();++i)
	{
		if(Particles(i)->isAlive)
		{
			Particles(i)->LifeTime -= TickN/1000.0f;
			Particles(i)->Position += Particles(i)->Velocity/10000.0f;

			if(Particles(i)->LifeTime < 0)
			{
				--CurrentParticleNum;
				Particles(i)->isAlive = false;
			}
		}
	}	
	TC = GetTickCount();*/
}

void CEmitter::PhysicsTick(unsigned long dTime)
{
}

void CEmitter::UpdateTransform()
{
}