#pragma once

#define generic GENERIC
#include "UWorld.h"
#undef GENERIC

class UMosesWorld : public UWorld
{
	DECLARE_CLASS(UMosesWorld,);
public:
	UMosesWorld();
	~UMosesWorld();

	class CBox* Box;						// ¹æ
	class CCharacter* Character;
	class CDirectionalLight* DirectionalLight;
	
	virtual bool InitializeWorld();
	virtual bool DestroyWorld();
	virtual bool Tick(DWORD dTime);	
};