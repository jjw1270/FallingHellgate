// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotSystem/QuickSlotComponent.h"
#include "FallingHellgate.h"
#include "ItemData.h"
#include "FHGameInstance.h"
#include "FHPlayerController.h"
#include "InventoryComponent.h"

UQuickSlotComponent::UQuickSlotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetComponentTickEnabled(false);
}

void UQuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();

	InitComponent();
}

void UQuickSlotComponent::InitComponent()
{
	AFHPlayerController* PC = Cast<AFHPlayerController>(GetOwner());
	CHECK_VALID(PC);

	GI = PC->GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	InventoryComp = PC->GetInventoryComp();
	CHECK_VALID(InventoryComp);
}

void UQuickSlotComponent::ManageQuickSlot(UItemData* TargetItemData, const int32& TargetItemAmount)
{
	// check item is already in quickslot
	// if true, Delete QuickSlot Item
	int32 ItemExistInQuickSlotIndex;
	if (IsItemExistInQuickSlot(TargetItemData, ItemExistInQuickSlotIndex))
	{
		DeleteItemFromQuickSlot(ItemExistInQuickSlotIndex);

		return;
	}

	// else Check QuickSlot is empty
	// If QuickSlot is Full, Do Nothing
	int32 QuickSlotIndex = GetEmptyQuickSlotSlotIndex();
	if (QuickSlotIndex < 0)
	{
		return;
	}

	// Set Item to QuickSlot
	SetItemToQuickSlot(QuickSlotIndex, TargetItemData, TargetItemAmount);
}

void UQuickSlotComponent::UseQuickSlotItem(const int32& TargetQuickSlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("UseQuickSlotItem : %d"), TargetQuickSlotIndex+1)
	
}

void UQuickSlotComponent::SetItemToQuickSlot(const int32& NewQuickSlotIndex, class UItemData* NewItemData, const int32& NewItemAmount)
{
	// Check item already exist in quick slots
	// if true, delete exist item.
	int32 ItemExistInQuickSlotIndex;
	if (IsItemExistInQuickSlot(NewItemData, ItemExistInQuickSlotIndex))
	{
		DeleteItemFromQuickSlot(ItemExistInQuickSlotIndex);
	}
	
	// if Item Exist in NewQuickSlotIndex, delete item
	DeleteItemFromQuickSlot(NewQuickSlotIndex);

	GI->GetQuickSlotItems()->Add(NewQuickSlotIndex, NewItemData);

	//BroadCast to Inventory Widget
	if (InventoryComp->ItemRegisterDelegate.IsBound())
	{
		InventoryComp->ItemRegisterDelegate.Broadcast(NewItemData, true);
	}

	//BroadCast to QuickSlot Widget
	if (QuickSlotUpdateDelegate.IsBound())
	{
		QuickSlotUpdateDelegate.Broadcast(NewQuickSlotIndex, NewItemData, NewItemAmount);
	}
}

void UQuickSlotComponent::DeleteItemFromQuickSlot(const int32& NewQuickSlotIndex)
{
	if (!GI->GetQuickSlotItems()->Contains(NewQuickSlotIndex))
	{
		return;
	}

	UItemData* DeleteItemData = *GI->GetQuickSlotItems()->Find(NewQuickSlotIndex);

	GI->GetQuickSlotItems()->Remove(NewQuickSlotIndex);

	//BroadCast to Inventory Widget
	if (InventoryComp->ItemRegisterDelegate.IsBound())
	{
		InventoryComp->ItemRegisterDelegate.Broadcast(DeleteItemData, false);
	}

	//BroadCast to QuickSlot Widget
	if (QuickSlotUpdateDelegate.IsBound())
	{
		QuickSlotUpdateDelegate.Broadcast(NewQuickSlotIndex, nullptr, 0);
	}
}

bool UQuickSlotComponent::IsQuickSlotEmpty(int32 QuickSlotIndex)
{
	for (const auto& MyQuickslot : *GI->GetQuickSlotItems())
	{
		if(MyQuickslot.Key == QuickSlotIndex)
		{
			return false;
		}
	}

	return true;
}

bool UQuickSlotComponent::IsItemExistInQuickSlot(UItemData* TargetItemData, int32& OutIndex)
{
	for (const auto& MyQuickslot : *GI->GetQuickSlotItems())
	{
		if (MyQuickslot.Value == TargetItemData)
		{
			OutIndex = MyQuickslot.Key;

			return true;
		}
	}

	return false;
}

int32 UQuickSlotComponent::GetEmptyQuickSlotSlotIndex()
{
	int32 EmptyIndex = -99;

	// Max Slot Length is 6
	for (int i = 0; i < 6; i++)
	{
		if (!GI->GetQuickSlotItems()->Contains(i))
		{
			EmptyIndex = i;

			break;
		}
	}

	return EmptyIndex;
}
