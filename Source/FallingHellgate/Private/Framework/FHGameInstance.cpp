// Fill out your copyright notice in the Description page of Project Settings.


#include "FHGameInstance.h"
#include "FallingHellgate.h"
#include "ItemDataManager.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "FHPlayerController.h"

UFHGameInstance::UFHGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UFHGameInstance::TESTss()
{
	UE_LOG(LogTemp, Warning, TEXT("<<<<Inventory Items>>>>"));
	for (const auto& Item : InventoryItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : %d [%d]"), *GetBaseItemData(Item.Key).Name, Item.Value, Item.Key);
	}
	UE_LOG(LogTemp, Warning, TEXT("--------------------------------------------"));

	UE_LOG(LogTemp, Warning, TEXT("<<<<QuickSlot Items>>>>"));
	for (const auto& Item : QuickSlotItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("Index[%d] : %s [%d]"), Item.Key, *GetBaseItemData(Item.Value).Name, Item.Value);
	}
	UE_LOG(LogTemp, Warning, TEXT("--------------------------------------------"));

	UE_LOG(LogTemp, Warning, TEXT("<<<<Equipment Items>>>>"));
	for (const auto& Item : EquipmentItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s [%d]"), *GetBaseItemData(Item).Name, Item);
	}
	UE_LOG(LogTemp, Warning, TEXT("--------------------------------------------"));

	UE_LOG(LogTemp, Warning, TEXT("<<<<Default Player Stats>>>>"));
	UE_LOG(LogTemp, Warning, TEXT("Health : [%d]"), DefaultPlayerStats.Health);
	UE_LOG(LogTemp, Warning, TEXT("Stamina : [%d]"), DefaultPlayerStats.Stamina);
	UE_LOG(LogTemp, Warning, TEXT("Attack : [%d]"), DefaultPlayerStats.Attack);
	UE_LOG(LogTemp, Warning, TEXT("AttackSpeed : [%f]"), DefaultPlayerStats.AttackSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Critical : [%f]"), DefaultPlayerStats.Critcal);
	UE_LOG(LogTemp, Warning, TEXT("Defence : [%d]"), DefaultPlayerStats.Defence);
	UE_LOG(LogTemp, Warning, TEXT("--------------------------------------------"));

}

FBaseItemData UFHGameInstance::GetBaseItemData(const int32& ItemID)
{
	FBaseItemData TempBaseItemData;

	FConsumableItemData TempConsumableItemData;
	FWeaponItemData TempWeaponItemData;
	FArmorItemData TempArmorItemData;

	int32 ItemPureID = UItemDataManager::GetPureID(ItemID);

	switch (UItemDataManager::GetItemType(ItemPureID))
	{
	case EItemType::Consumable:
		GetConsumableItemInfo(ItemPureID, TempConsumableItemData);
		TempBaseItemData = TempConsumableItemData.BaseData;
		break;
	case EItemType::Weapon:
		GetWeaponItemInfo(ItemPureID, TempWeaponItemData);
		TempBaseItemData = TempWeaponItemData.BaseData;
		break;
	case EItemType::Armor:
		GetArmorItemInfo(ItemPureID, TempArmorItemData);
		TempBaseItemData = TempArmorItemData.BaseData;
		break;
	default:
		break;
	}

	return TempBaseItemData;
}

bool UFHGameInstance::GetConsumableItemInfo(const int32& PureItemID, FConsumableItemData& OutData)
{
	if (!ConsumableItemDataTable)
	{
		return false;
	}

	FConsumableItemData* pData = ConsumableItemDataTable->FindRow<FConsumableItemData>(*FString::FromInt(PureItemID), TEXT(""), false);

	if (pData == nullptr)
	{
		return false;
	}

	OutData = *pData;
	return true;
}

bool UFHGameInstance::GetWeaponItemInfo(const int32& PureItemID, FWeaponItemData& OutData)
{
	if (!WeaponItemDataTable)
	{
		return false;
	}

	FWeaponItemData* pData = WeaponItemDataTable->FindRow<FWeaponItemData>(*FString::FromInt(PureItemID), TEXT(""), false);

	if (pData == nullptr)
	{
		return false;
	}

	OutData = *pData;
	return true;
}

bool UFHGameInstance::GetArmorItemInfo(const int32& PureItemID, FArmorItemData& OutData)
{
	if (!ArmorItemDataTable)
	{
		return false;
	}

	FArmorItemData* pData = ArmorItemDataTable->FindRow<FArmorItemData>(*FString::FromInt(PureItemID), TEXT(""), false);

	if (pData == nullptr)
	{
		return false;
	}

	OutData = *pData;
	return true;
}
