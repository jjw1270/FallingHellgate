// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Math/UnrealMathUtility.h"
#include "FallingHellgate.h"
#include "ItemData.h"
#include "ItemDataManager.h"

#include "FHGameInstance.h"
#include "FHPlayerController.h"

#include "QuickSlotComponent.h"
#include "EquipmentComponent.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetComponentTickEnabled(false);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InitComponent();
}

void UInventoryComponent::InitComponent()
{
	AFHPlayerController* PC = Cast<AFHPlayerController>(GetOwner());
	CHECK_VALID(PC);

	GI = PC->GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	QuickSlotComp = PC->GetQuickSlotComp();
	CHECK_VALID(QuickSlotComp);

	EquipComp = PC->GetEquipmentComp();
	CHECK_VALID(EquipComp);
}

void UInventoryComponent::AddItemToInventory(const int32& NewItemID, const int32& NewAmount)
{
	// If Item is Equipment Item, Add new item with unique id
	// else, Already Exist in Inventory, Add Amount

	EItemType NewItemType = GI->GetItemDataManager()->GetItemType(NewItemID);
	UItemData* NewItem;

	switch (NewItemType)
	{
	case EItemType::Weapon:
		NewItem = GetWeaponItemData(NewItemID);
		CHECK_VALID(NewItem);

		GI->GetInventoryItems()->Add(NewItem, NewAmount);

		if (ItemUpdateDelegate.IsBound())
		{
			ItemUpdateDelegate.Broadcast(NewItem, NewAmount);
		}

		return;
	case EItemType::Armor:
		NewItem = GetArmorItemData(NewItemID);
		CHECK_VALID(NewItem);

		GI->GetInventoryItems()->Add(NewItem, NewAmount);

		if (ItemUpdateDelegate.IsBound())
		{
			ItemUpdateDelegate.Broadcast(NewItem, NewAmount);
		}

		return;
	case EItemType::Consumable:
		// Check Item is already in Inventory and add Amount if true
		for (auto& MyItem : *GI->GetInventoryItems())
		{
			FBaseItemData BaseItemData = MyItem.Key->GetBaseData();

			if (BaseItemData.ID == NewItemID)
			{
				MyItem.Value += NewAmount;

				if (ItemUpdateDelegate.IsBound())
				{
					ItemUpdateDelegate.Broadcast(MyItem.Key, MyItem.Value);
				}

				return;
			}
		}

		// else Make New Item
		NewItem = GetConsumableItemData(NewItemID);
		CHECK_VALID(NewItem);

		GI->GetInventoryItems()->Add(NewItem, NewAmount);

		if (ItemUpdateDelegate.IsBound())
		{
			ItemUpdateDelegate.Broadcast(NewItem, NewAmount);
		}

		return;
	case EItemType::Others:
		// To be implemented :)
		break;
	default:
		break;
	}
}

void UInventoryComponent::RemoveItemFromInventory(class UItemData* ItemData, const int32& Amount)
{
	for (auto& MyItem : *GI->GetInventoryItems())
	{
		if (MyItem.Key == ItemData)
		{
			MyItem.Value -= Amount;

			// BroadCast
			if (ItemUpdateDelegate.IsBound())
			{
				ItemUpdateDelegate.Broadcast(MyItem.Key, MyItem.Value);
			}

			if (MyItem.Value <= 0)
			{
				GI->GetInventoryItems()->Remove(MyItem.Key);
			}

			return;
		}
	}
}

int32 UInventoryComponent::MakeUniqueID()
{
	while (true)
	{
		bool bSuccess = true;
		int32 TempID = FMath::RandRange(1001, 9999);

		for (auto& Item : *GI->GetInventoryItems())
		{
			if (Item.Key->GetUniqueID() == TempID)
			{
				bSuccess = false;
				break;
			}
		}

		if (bSuccess)
		{
			return TempID;
		}
	}
}

void UInventoryComponent::ManageItem(class UItemData* TargetItemData, const int32& TargetItemAmount)
{
	FBaseItemData BaseItemData = TargetItemData->GetBaseData();

	// Consumable Items
	if (BaseItemData.Type == EItemType::Consumable)
	{
		QuickSlotComp->ManageQuickSlot(TargetItemData, TargetItemAmount);

		return;
	}

	// Weapon, Armor Items
	if (BaseItemData.Type == EItemType::Weapon || BaseItemData.Type == EItemType::Armor)
	{
		EquipComp->ManageEquipment(TargetItemData);

		return;
	}
}

UItemData* UInventoryComponent::GetWeaponItemData(const int32& TargetItemID)
{
	FWeaponItemData TempWeaponItemData;

	if (!GI->GetItemDataManager()->GetWeaponItemInfo(TargetItemID, TempWeaponItemData))
	{
		UE_LOG(LogTemp, Error, TEXT("Data in WeaponItemDataTable is Missing"));
		return nullptr;
	}

	UItemData* NewItem = NewObject<UItemData>(this, UItemData::StaticClass());
	if (NewItem)
	{
		NewItem->SetWeaponData(TempWeaponItemData, MakeUniqueID());

		return NewItem;
	}

	return nullptr;
}

UItemData* UInventoryComponent::GetArmorItemData(const int32& TargetItemID)
{
	FArmorItemData TempArmorItemData;

	if (!GI->GetItemDataManager()->GetArmorItemInfo(TargetItemID, TempArmorItemData))
	{
		UE_LOG(LogTemp, Error, TEXT("Data in ArmorItemDataTable is Missing"));
		return nullptr;
	}

	UItemData* NewItem = NewObject<UItemData>(this, UItemData::StaticClass());
	if (NewItem)
	{
		NewItem->SetArmorData(TempArmorItemData, MakeUniqueID());

		return NewItem;
	}

	return nullptr;
}

UItemData* UInventoryComponent::GetConsumableItemData(const int32& TargetItemID)
{
	FConsumableItemData TempConsumableItemData;

	if (!GI->GetItemDataManager()->GetConsumableItemInfo(TargetItemID, TempConsumableItemData))
	{
		UE_LOG(LogTemp, Error, TEXT("Data in ConsumableItemDataTable is Missing"));
		return nullptr;
	}

	UItemData* NewItem = NewObject<UItemData>(this, UItemData::StaticClass());
	if (NewItem)
	{
		NewItem->SetConsumableData(TempConsumableItemData);

		return NewItem;
	}

	return nullptr;
}
