// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ApplyDamage.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UAnimNotifyState_ApplyDamage : public UAnimNotifyState
{
	GENERATED_BODY()
	

public:
	UAnimNotifyState_ApplyDamage(const FObjectInitializer& ObjectInitializer);

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	bool DetectWeaponCollsion(TArray<FHitResult>& OutResults);

	void AddDamageToTargets(TArray<FHitResult>& OutResults);

protected:
	//Attack Collision
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName CollisionAttachSocket1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName CollisionAttachSocket2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TraceSphereRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageCoefficient{ 1.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugCollision;

protected:
	UPROPERTY()
	class AFHPlayerCharacter* OwnCharacter;

	class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY()
	TArray<AActor*> AlreadyDamagedTargets;

};
