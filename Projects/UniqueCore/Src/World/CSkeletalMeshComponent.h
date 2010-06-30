#pragma once

#include "TDataTypes.h"
#include "BComponent.h"

class CSkeletalMeshComponent : public BComponent
{
public:
	CSkeletalMeshComponent(class BThing* InOwner);
	virtual ~CSkeletalMeshComponent();

	virtual void UpdateComponent();
};
