#pragma once

#include "TDataTypes.h"
#include "BComponent.h"

class CSkeletalMeshComponent : public BComponent
{
public:
	CSkeletalMeshComponent();
	virtual ~CSkeletalMeshComponent();

	virtual void UpdateComponent();
};
