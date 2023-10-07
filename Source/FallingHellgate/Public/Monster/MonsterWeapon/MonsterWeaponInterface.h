// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MonsterWeaponInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMonsterWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FALLINGHELLGATE_API IMonsterWeaponInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MonsterInterface")
	void EventTrigger();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MonsterInterface")
	void EventAttack();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "MonsterInterface")
	void EventPhase();
};
