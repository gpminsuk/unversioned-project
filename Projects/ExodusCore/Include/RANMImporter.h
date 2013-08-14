#pragma once

#include "RImporter.h"

class RANMImporter: public RImporter
{
public:
	virtual bool Import(TString& Filename, class RAnimationSequence*& AnimationSequence, class RBoneHierarchy*& BoneHierarchy, class RMesh*& Model);
};

