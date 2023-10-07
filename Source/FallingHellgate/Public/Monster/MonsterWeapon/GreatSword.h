// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GreatSword.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API AGreatSword : public AWeapon
{
	GENERATED_BODY()

public:
	AGreatSword();

public:

	virtual void EventAttack_Implementation() override;

	UFUNCTION(Server, Reliable, Category = "Monster")
	void ReqAttack(FVector vStart, FVector vEnd);
};
