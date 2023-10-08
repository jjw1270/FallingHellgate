// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataManager.h"
#include "FallingHellgate.h"


EItemType UItemDataManager::GetItemType(const int32& ItemID)
{
	EItemType Itemtype = EItemType::None;

	int32 ItemTypeNum = ItemID / 10000;

	if (ItemTypeNum > 4)
	{
		ItemTypeNum = GetPureID(ItemID) / 10000;
	}

	switch (ItemTypeNum)
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

int32 UItemDataManager::GetPureID(const int32& ItemID)
{
	FString TempItemIDStr = FString::FromInt(ItemID);

	int32 PureItemID = FCString::Atoi(*TempItemIDStr.LeftChop(4));

	return PureItemID;
}

int32 UItemDataManager::GetUniqueID(const int32& ItemID)
{
	FString TempItemIDStr = FString::FromInt(ItemID);

	int32 UniqueID = FCString::Atoi(*TempItemIDStr.Right(3));

	return UniqueID;
}

bool UItemDataManager::IsRegistered(const int32& ItemID)
{
	FString TempItemIDStr = FString::FromInt(ItemID);

	if (TempItemIDStr.Len() >= 4) {
		int32 RegisterIndex = TempItemIDStr.Len() - 4;

		return TempItemIDStr.Mid(RegisterIndex, 1).ToBool();
	}
	else
	{
		return false;
	}
}

void UItemDataManager::RegistItem(int32& ItemID)
{
	FString TempItemIDStr = FString::FromInt(ItemID);

	bool bIsRegisted = false;
	int32 RegisterIndex = -1;

	if (TempItemIDStr.Len() >= 4) {
		RegisterIndex = TempItemIDStr.Len() - 4;

		bIsRegisted = TempItemIDStr.Mid(RegisterIndex, 1).ToBool();
	}

	if (RegisterIndex < 0)
	{
		return;
	}

	if (bIsRegisted)
	{
		TempItemIDStr[RegisterIndex] = TEXT('0');
	}
	else
	{
		TempItemIDStr[RegisterIndex] = TEXT('1');
	}

	ItemID = FCString::Atoi(*TempItemIDStr);
}
