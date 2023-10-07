// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Spear.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API ASpear : public AWeapon
{
	GENERATED_BODY()

public:
	ASpear();

public:

	virtual void EventAttack_Implementation() override;

	UFUNCTION(Server, Reliable, Category = "MonsterWeapon")
	void ReqAttack(FVector vStart, FVector vEnd);
	
};
