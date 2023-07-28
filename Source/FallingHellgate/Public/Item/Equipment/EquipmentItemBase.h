// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "EquipmentItemBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FALLINGHELLGATE_API UEquipmentItemBase : public UItemBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Infomation")
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Infomation")
	int32 Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Infomation")
	int32 AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Infomation")
	int32 AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Infomation")
	int32 CriticalChance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Infomation")
	int32 DefensivePower;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetHealth() { return Health; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetStamina() { return Stamina; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetAttackPower() { return AttackPower; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetAttackSpeed() { return AttackSpeed; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetCriticalChance() { return CriticalChance; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetDefensivePower() { return DefensivePower; }

};
