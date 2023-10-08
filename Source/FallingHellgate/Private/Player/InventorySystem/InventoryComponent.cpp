// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Math/UnrealMathUtility.h"
#include "FallingHellgate.h"

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

	ItemRegisterDelegate.AddUObject(this, &UInventoryComponent::OnItemRegister);

	FTimerHandle TempHandle;
	GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &UInventoryComponent::UpdateInventory, 2.f, false);
}

void UInventoryComponent::UpdateInventory()
{
	for (const auto& MyItem : *GI->GetInventoryItems())
	{
		UE_LOG(LogTemp, Warning, TEXT("Broadcast %d"), MyItem.Key);

		ItemUpdateDelegate.Broadcast(MyItem.Key, MyItem.Value);
	}
}

void UInventoryComponent::OnItemRegister(const int32& UpdateItemID, const bool& bIsRegist)
{
	CHECK_VALID(GI);

	UE_LOG(LogTemp, Warning, TEXT("OnItemRegister %d"), UpdateItemID);

	int32 UpdateItemAmount;
	GI->GetInventoryItems()->RemoveAndCopyValue(UpdateItemID, UpdateItemAmount);

	int32 RegistedItemID = UpdateItemID;
	UItemDataManager::RegistItem(RegistedItemID);

	GI->GetInventoryItems()->Add(RegistedItemID, UpdateItemAmount);
}

// ItemID = ID+Regist(0/1)+UniqueID(1000/1001~9999)
void UInventoryComponent::AddItemToInventory(const int32& NewPureItemID, const int32& NewAmount)
{
	// If Item is Equipment Item, Add new item with unique id
	// else, Already Exist in Inventory, Add Amount

	int32 NewItemID = 0;

	FString TempItemIDString = FString::FromInt(NewPureItemID);

	TempItemIDString.Append("0"); // Num Register

	switch (UItemDataManager::GetItemType(NewPureItemID))
	{
	case EItemType::Weapon:
	case EItemType::Armor:
		TempItemIDString.AppendInt(MakeUniqueID(NewPureItemID));

		NewItemID = FCString::Atoi(*TempItemIDString);
		UE_LOG(LogTemp, Warning, TEXT("Add Equip Item To Inventory %d"), NewItemID);

		GI->GetInventoryItems()->Add(NewItemID, NewAmount);

		if (ItemUpdateDelegate.IsBound())
		{
			ItemUpdateDelegate.Broadcast(NewItemID, NewAmount);
		}

		return;
	case EItemType::Consumable:
		// Check Item is already in Inventory and add Amount if true
		for (auto& MyItem : *GI->GetInventoryItems())
		{
			if (UItemDataManager::GetPureID(MyItem.Key) == NewPureItemID)
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
		TempItemIDString.AppendInt(100);
		NewItemID = FCString::Atoi(*TempItemIDString);

		GI->GetInventoryItems()->Add(NewItemID, NewAmount);

		if (ItemUpdateDelegate.IsBound())
		{
			ItemUpdateDelegate.Broadcast(NewItemID, NewAmount);
		}

		return;
	case EItemType::Others:
		// Reserved
		break;
	default:
		break;
	}
}

void UInventoryComponent::RemoveItemFromInventory(const int32& TargetItemID, const int32& Amount)
{
	for (auto& MyItem : *GI->GetInventoryItems())
	{
		if (MyItem.Key == TargetItemID)
		{
			MyItem.Value -= Amount;

			// BroadCast
			if (ItemUpdateDelegate.IsBound())
			{
				ItemUpdateDelegate.Broadcast(TargetItemID, MyItem.Value);
			}

			if (MyItem.Value <= 0)
			{
				GI->GetInventoryItems()->Remove(TargetItemID);
			}

			return;
		}
	}
}

int32 UInventoryComponent::MakeUniqueID(const int32& NewPureItemID)
{
	while (true)
	{
		bool bSuccess = true;
		int32 TempUniqueID = FMath::RandRange(101, 999);

		if (GI->GetInventoryItems()->Num() > 0)
		{
			for (const auto& Item : *GI->GetInventoryItems())
			{
				if (UItemDataManager::GetPureID(Item.Key) != NewPureItemID)
				{
					continue;
				}

				if (UItemDataManager::GetUniqueID(Item.Key) == TempUniqueID)
				{
					bSuccess = false;
					break;
				}
			}
		}

		if (bSuccess)
		{
			return TempUniqueID;
		}
	}
}

void UInventoryComponent::ManageItem(const int32& TargetItemID, const int32& TargetItemAmount)
{
	switch (UItemDataManager::GetItemType(TargetItemID))
	{
	case EItemType::Consumable:
		QuickSlotComp->ManageQuickSlot(TargetItemID, TargetItemAmount);
		break;
	case EItemType::Weapon:
	case EItemType::Armor:
		EquipComp->ManageEquipment(TargetItemID);
		break;
	default:
		break;
	}
}
