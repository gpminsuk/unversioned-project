#include "stdafx.h"
#include "RAnimationTree.h"
#include "RAnimationSequence.h"
#include "CSkeletalMeshPrimitive.h"
#include "BThing.h"

IMPLEMENT_CLASS(RAnimationTree);
IMPLEMENT_CLASS(RAnimationNode);
IMPLEMENT_CLASS(RAnimationSequenceNode);
IMPLEMENT_CLASS(RAnimationStateBlend);
IMPLEMENT_CLASS(RAnimationBlend);

void RAnimationTree::CalcBoneMatrices(TArray<TBone*>& Bones, float Weight) {
	Root->CalcBoneMatrices(Bones, 1.0f);
}

void RAnimationTree::Tick(float Time) {
	Root->Tick(Time);
}

// 임시 코드
#include "RANMImporter.h"

RAnimationSequenceNode* NewAnimaionSeqNode(RBoneHierarchy* B, TString s) {
	RANMImporter Importer;
	RMesh* M;
	RAnimationSequence* Seq;
	RAnimationSequenceNode* SeqNode = new RAnimationSequenceNode();
	Importer.Import(s, Seq, B, M);
	SeqNode->AnimationSequence.Set(Seq);
	SeqNode->Activated();
	return SeqNode;
}

void RAnimationBlend::Initialize(BThing* T, RBoneHierarchy* B) {
	TargetIndex = 0;

	RAnimationStateBlend* StateBlend = new RAnimationStateBlend();
	StateBlend->Initialize(T);
	Children.AddItem(StateBlend);
	Weights.AddItem(1.0f);
	
	StateBlend->TargetIndex = 0;
	StateBlend->Children.AddItem(NewAnimaionSeqNode(B, TString("..\\..\\Resources\\Renekton_Idle1.anm")));
	StateBlend->Weights.AddItem(1.0f);
	StateBlend->BlendRates.AddItem(0.0f);

	StateBlend->Children.AddItem(NewAnimaionSeqNode(B, TString("..\\..\\Resources\\renekton_run.anm")));
	StateBlend->Weights.AddItem(0.0f);
	StateBlend->BlendRates.AddItem(0.0f);
}

void RAnimationBlend::SetActiveNode(int NodeIndex, float BlendTime) {
	if(TargetIndex != NodeIndex) {
		BlendRates(TargetIndex) = -1.0f / BlendTime;
		TargetIndex = NodeIndex;
		BlendRates(TargetIndex) = 1.0f / BlendTime;
	}
}

void RAnimationBlend::Tick(float Time) {
	for(unsigned int i=0;i<BlendRates.Size();++i) {
		Weights(i) += BlendRates(i) * Time;
		if(Weights(i) > 1.0f) {
			BlendRates(i) = 0.0f;
			Weights(i) = 1.0f;
		}
		else if(Weights(i) < 0.0f) {
			BlendRates(i) = 0.0f;
			Weights(i) = 0.0f;
		}
	}
	for(unsigned int i=0;i<Weights.Size();++i) {
		Children(i)->Tick(Time);
	}
}

void RAnimationBlend::CalcBoneMatrices(TArray<TBone*>& Bones, float Weight) {
	for(unsigned int i=0;i<Weights.Size();++i) {
		Children(i)->CalcBoneMatrices(Bones, Weight * Weights(i));
	}
}

void RAnimationBlend::Activated() {

}

void RAnimationBlend::Deactivated() {

}

void RAnimationSequenceNode::CalcBoneMatrices(TArray<TBone*>& Bones, float Weight) {
	for(unsigned int i=0;i<Bones.Size();++i) {
		Bones(i)->CalcBoneMatrix(AnimationSequence.Get()->AnimationBoneSequences(i), ElapsedTime, Weight);
	}
}

void RAnimationSequenceNode::Tick(float Time) {
	// 임시
	ElapsedTime += Time * 24.0f;
	if(ElapsedTime > AnimationSequence.Get()->AnimationBoneSequences(0)->PosKeys.Size()) {
		ElapsedTime = 0;
	}
}

void RAnimationSequenceNode::Activated() {
	ElapsedTime = 0;
}

void RAnimationSequenceNode::Deactivated() {

}

void RAnimationStateBlend::Initialize(BThing* InOwner) {
	Owner = InOwner;
}

void RAnimationStateBlend::Tick(float Time) {
	__super::Tick(Time);
	if(Owner->m_IsMoving) {
		SetActiveNode(1, 0.3f);
	}
	else {
		SetActiveNode(0, 0.3f);
	}
}