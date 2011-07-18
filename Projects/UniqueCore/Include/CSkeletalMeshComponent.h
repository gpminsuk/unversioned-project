#pragma once

#include "TDataTypes.h"
#include "BComponent.h"

class CSkeletalMeshComponent : public BComponent
{
	DECLARE_CLASS(CSkeletalMeshComponent,)
public:
	CSkeletalMeshComponent();
	virtual ~CSkeletalMeshComponent();

	virtual void UpdateComponent();

	void PlayAnimation(int StartTime, bool Loop);
	void StopAnimation();
};
