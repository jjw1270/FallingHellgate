// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ConsumableItemBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FALLINGHELLGATE_API UConsumableItemBase : public UItemBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Infomation")
	bool bIsEffectSelf;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Infomation")
	bool bHasDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Infomation")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Infomation")
	float EffectValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Infomation")
	int32 MaxEquipNumber;

public:
	void ItemEffect();

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsEffectSelf() { return bIsEffectSelf; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetHasDuration() { return bHasDuration; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetDuration() { return Duration; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetEffectValue() { return EffectValue; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetMaxEquipNumber() { return MaxEquipNumber; }

};
