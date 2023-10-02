// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_UseItem.h"
#include "FallingHellgate.h"
#include "FHPlayerController.h"
#include "QuickSlotComponent.h"

UAnimNotify_UseItem::UAnimNotify_UseItem(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldFireInEditor = false;
}

void UAnimNotify_UseItem::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AFHPlayerController* PC = MeshComp->GetOwner()->GetInstigatorController<AFHPlayerController>();
	CHECK_VALID(PC);

	UQuickSlotComponent* QuickSlotComp = PC->GetQuickSlotComp();
	CHECK_VALID(QuickSlotComp);

	QuickSlotComp->UseQuickSlotItem();
}
