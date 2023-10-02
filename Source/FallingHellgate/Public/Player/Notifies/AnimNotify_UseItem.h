// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_UseItem.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UAnimNotify_UseItem : public UAnimNotify
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
