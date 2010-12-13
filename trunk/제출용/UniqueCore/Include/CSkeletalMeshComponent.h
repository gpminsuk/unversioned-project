#pragma once

#include "TDataTypes.h"
#include "BComponent.h"

class CSkeletalMeshComponent : public BComponent
{
public:
	CSkeletalMeshComponent(int i);
	virtual ~CSkeletalMeshComponent();

	virtual void UpdateComponent();

	void PlayAnimation(int StartTime, bool Loop);
	void StopAnimation();
};
