#pragma once

#include "RImporter.h"

class RASEImporter : public RImporter
{
public:
	virtual bool Import(TString& Filename, class RAnimationSequence*& AnimationSequence, class RBoneHierarchy*& BoneHierarchy, class RSkeletalMesh*& Model);
};