// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemDataManager.generated.h"

// ItemID = ID+Regist(0/1)+UniqueID(100/101~999)
UCLASS()
class FALLINGHELLGATE_API UItemDataManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Item Data")
	static int32 GetPureID(const int32& ItemID);

	UFUNCTION(BlueprintCallable, Category = "Item Data")
	static int32 GetUniqueID(const int32& ItemID);

	UFUNCTION(BlueprintCallable, Category = "Item Data")
	static enum EItemType GetItemType(const int32& ItemID);

	UFUNCTION(BlueprintCallable, Category = "Item Data")
	static bool IsRegistered(const int32& ItemID);

public:
	UFUNCTION(BlueprintCallable, Category = "Item Data")
	static void RegistItem(int32& ItemID);
};
