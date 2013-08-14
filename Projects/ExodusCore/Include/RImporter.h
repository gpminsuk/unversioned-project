#pragma once

#include "TDataTypes.h"
#include "RResource.h"

class AObject;

class RImporter
{
public:
	virtual bool Import(TString& Filename, class RAnimationSequence*& AnimationSequence, class RBoneHierarchy*& BoneHierarchy, class RMesh*& Model) = 0;
};
