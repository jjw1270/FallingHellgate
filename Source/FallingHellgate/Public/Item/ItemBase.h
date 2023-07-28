// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FALLINGHELLGATE_API UItemBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Infomation")
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Infomation")
	TObjectPtr<UMaterialInstance> ItemImage;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetPrice() { return Price; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UMaterialInstance* GetItemImage() { return ItemImage; }

};
