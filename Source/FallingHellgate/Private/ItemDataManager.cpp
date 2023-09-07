// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataManager.h"
#include "FallingHellgate.h"

bool UItemDataManager::GetConsumableItemInfo(const int32& ItemID, FConsumableItemData& OutData)
{
	check(ConsumableItemDataTable);

	FConsumableItemData* pData = ConsumableItemDataTable->FindRow<FConsumableItemData>(*FString::FromInt(ItemID), TEXT(""), false);

	if (pData == nullptr)
	{
		return false;
	}

	OutData = *pData;
	return true;
}

bool UItemDataManager::GetWeaponItemInfo(const int32& ItemID, FWeaponItemData& OutData)
{
	check(WeaponItemDataTable);

	FWeaponItemData* pData = WeaponItemDataTable->FindRow<FWeaponItemData>(*FString::FromInt(ItemID), TEXT(""), false);

	if (pData == nullptr)
	{
		return false;
	}

	OutData = *pData;
	return true;
}

bool UItemDataManager::GetArmorItemInfo(const int32& ItemID, FArmorItemData& OutData)
{
	check(ArmorItemDataTable);

	FArmorItemData* pData = ArmorItemDataTable->FindRow<FArmorItemData>(*FString::FromInt(ItemID), TEXT(""), false);

	if (pData == nullptr)
	{
		return false;
	}

	OutData = *pData;
	return true;
}

EItemType UItemDataManager::GetItemType(const int32& ItemID)
{
	EItemType Itemtype = EItemType::None;

	switch (ItemID / 10000)
	{
	case 1:
		Itemtype = EItemType::Consumable;
		break;
	case 2:
		Itemtype = EItemType::Weapon;
		break;
	case 3:
		Itemtype = EItemType::Armor;
		break;
	case 4:
		Itemtype = EItemType::Others;
		break;
	default:
		break;
	}

	return Itemtype;
}
