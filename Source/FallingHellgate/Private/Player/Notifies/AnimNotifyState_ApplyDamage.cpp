// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_ApplyDamage.h"

UAnimNotifyState_ApplyDamage::UAnimNotifyState_ApplyDamage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//Do not Active Notify State in UE Editor(In Animation Montage Window)
	//If Do not this, UE Editor Maybe crashed
	bShouldFireInEditor = false;
}

void UAnimNotifyState_ApplyDamage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UAnimNotifyState_ApplyDamage::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}

void UAnimNotifyState_ApplyDamage::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
