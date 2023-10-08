// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataManager.h"
#include "ItemData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FALLINGHELLGATE_API UItemData : public UObject
{
	GENERATED_BODY()
	
public:
	UItemData();

//public:
//	bool operator==(const UItemData& Other) const
//	{
//		if (UniqueID > 1000)
//		{
//			return BaseData.ID == Other.BaseData.ID && UniqueID == Other.UniqueID;
//		}
//
//		return BaseData.ID == Other.BaseData.ID;
//	}
//
//protected:
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data)
//	FConsumableItemData ConsumableData;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data)
//	FWeaponItemData WeaponData;
//	
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data)
//	FArmorItemData ArmorData;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data)
//	FBaseItemData BaseData;
//
//	// UniqueID for Prevent Item Data Overlaping ( 1001 ~ 9999 )
//	// Only for NonStackable Items!
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data)
//	int32 UniqueID;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Data)
//	bool bIsRegisted;
//
//public:
//	void SetConsumableData(FConsumableItemData& NewConsumableData);
//
//	void SetWeaponData(FWeaponItemData& NewWeaponData, int32 NewUniqueID);
//
//	void SetArmorData(FArmorItemData& NewArmorData, int32 NewUniqueID);
//
//	void RegistItem(bool bRegist);
//
//public:
//	UFUNCTION(BlueprintCallable, Category = Data)
//	FBaseItemData& GetBaseData();
//
//	UFUNCTION(BlueprintCallable, Category = Data)
//	EItemType GetItemType();
//
//	UFUNCTION(BlueprintCallable, Category = Data)
//	bool GetConsumableData(FConsumableItemData& OutData);
//
//	UFUNCTION(BlueprintCallable, Category = Data)
//	bool GetWeaponData(FWeaponItemData& OutData);
//
//	UFUNCTION(BlueprintCallable, Category = Data)
//	bool GetArmorData(FArmorItemData& OutData);
//
//	UFUNCTION(BlueprintCallable, Category = Data)
//	FORCEINLINE int32 GetUniqueID() const { return UniqueID; }
//
//	UFUNCTION(BlueprintCallable, Category = Data)
//	FORCEINLINE bool IsRegisted() const { return bIsRegisted; }
};
