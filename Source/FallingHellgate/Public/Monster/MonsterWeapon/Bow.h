// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Bow.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API ABow : public AWeapon
{
	GENERATED_BODY()

public:
	ABow();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterWeapon")
	class USkeletalMeshComponent* BowMesh;

	virtual void EventAttack_Implementation() override;

	UFUNCTION(Server, Reliable, Category = "MonsterWeapon")
	void ReqAttack(FVector vStart, FVector vEnd);
};
