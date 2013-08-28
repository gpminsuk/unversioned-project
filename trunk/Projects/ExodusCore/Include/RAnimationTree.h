#pragma once

#include "AObject.h"
#include "RAsset.h"
#include "TDataTypes.h"

class RAnimationSequence;
class TBone;
class BThing;

class RAnimationNode: public AObject {
	DECLARE_CLASS(RAnimationNode,CLASS_Abstract)
public:

	virtual void Tick(float Time) = 0;
	virtual void CalcBoneMatrices(TArray<TBone*>& Bones, float Weight) = 0;
	virtual void Activated() = 0;
	virtual void Deactivated() = 0;
};

class RAnimationSequenceNode: public RAnimationNode {
	DECLARE_CLASS(RAnimationSequenceNode,)
public:
	float ElapsedTime;

	RAssetPtr<RAnimationSequence> AnimationSequence;

	virtual void Tick(float Time);
	virtual void CalcBoneMatrices(TArray<TBone*>& Bones, float Weight);
	virtual void Activated();
	virtual void Deactivated();
};

class RAnimationBlend: public RAnimationNode {
	DECLARE_CLASS(RAnimationBlend,)
public:
	int TargetIndex;
	TArray<RAnimationNode*> Children;
	TArray<float> Weights;
	TArray<float> BlendRates;

	void Initialize(class BThing* T, class RBoneHierarchy* B);
	
	virtual void SetActiveNode(int NodeIndex, float BlendTime);
	virtual void Tick(float Time);
	virtual void CalcBoneMatrices(TArray<TBone*>& Bones, float Weight);
	virtual void Activated();
	virtual void Deactivated();
};

class RAnimationStateBlend: public RAnimationBlend {
	DECLARE_CLASS(RAnimationStateBlend,)
public:
	BThing* Owner;

	void Initialize(BThing* InOwner);

	virtual void Tick(float Time);
};

class RAnimationTree: public RAnimationNode
{
	DECLARE_CLASS(RAnimationTree,)
public:
	RAnimationNode* Root;

	virtual void Tick(float Time);
	virtual void CalcBoneMatrices(TArray<TBone*>& Bones, float Weight);
	virtual void Activated() {}
	virtual void Deactivated() {}
};
