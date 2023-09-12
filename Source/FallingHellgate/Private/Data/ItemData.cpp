// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData.h"
#include "FallingHellgate.h"

UItemData::UItemData()
{
}

void UItemData::SetConsumableData(FConsumableItemData& NewConsumableData)
{
	ConsumableData = NewConsumableData;

	BaseData = ConsumableData.BaseData;
}

void UItemData::SetWeaponData(FWeaponItemData& NewWeaponData, int32 NewUniqueID)
{
	WeaponData = NewWeaponData;

	UniqueID = NewUniqueID;

	BaseData = WeaponData.BaseData;
}

void UItemData::SetArmorData(FArmorItemData& NewArmorData, int32 NewUniqueID)
{
	ArmorData = NewArmorData;

	UniqueID = NewUniqueID;

	BaseData = ArmorData.BaseData;
}

FBaseItemData& UItemData::GetBaseData()
{
	return BaseData;
}

EItemType UItemData::GetItemType()
{
	if (ConsumableData.IsValid())
	{
		return ConsumableData.BaseData.Type;
	}
	if (WeaponData.IsValid())
	{
		return WeaponData.BaseData.Type;
	}
	if (ArmorData.IsValid())
	{
		return ArmorData.BaseData.Type;
	}

	return EItemType::None;
}

bool UItemData::GetConsumableData(FConsumableItemData& OutData)
{
	if (!ConsumableData.IsValid())
	{
		return false;
	}

	OutData = ConsumableData;
	return true;
}

bool UItemData::GetWeaponData(FWeaponItemData& OutData)
{
	if (!WeaponData.IsValid())
	{
		return false;
	}

	OutData = WeaponData;
	return true;
}

bool UItemData::GetArmorData(FArmorItemData& OutData)
{
	if (!ArmorData.IsValid())
	{
		return false;
	}

	OutData = ArmorData;
	return true;
}
