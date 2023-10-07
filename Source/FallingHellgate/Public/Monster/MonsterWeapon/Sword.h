// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Sword.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API ASword : public AWeapon
{
	GENERATED_BODY()

public:
	ASword();

public:

	virtual void EventAttack_Implementation() override;

	UFUNCTION(Server, Reliable, Category = "MonsterWeapon")
	void ReqAttack(FVector vStart, FVector vEnd);
};
