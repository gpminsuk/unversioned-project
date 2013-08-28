#pragma once

#include "RImporter.h"

class RSCBImporter: public RImporter
{
public:
	virtual bool Import(TString& Filename, class RAnimationSequence*& AnimationSequence, class RBoneHierarchy*& BoneHierarchy, class RMesh*& Model);
};

