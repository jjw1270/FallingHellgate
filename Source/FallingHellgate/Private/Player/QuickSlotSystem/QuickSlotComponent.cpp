// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotSystem/QuickSlotComponent.h"
#include "FallingHellgate.h"
#include "ItemDataManager.h"
#include "FHGameInstance.h"
#include "FHPlayerController.h"
#include "FHPlayerCharacter.h"
#include "PlayerStatusComponent.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

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
	PC = Cast<AFHPlayerController>(GetOwner());
	CHECK_VALID(PC);

	GI = PC->GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	InventoryComp = PC->GetInventoryComp();
	CHECK_VALID(InventoryComp);

	//QuickSlotUpdateDelegate.AddUObject(this, &UQuickSlotComponent::OnQuickSLotUpdate);

	FTimerHandle TempHandle;
	GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &UQuickSlotComponent::UpdateQuickSlot, 1.f, false);
}

//void UQuickSlotComponent::OnQuickSLotUpdate(const int32& UpdateSlotIdx, const int32& SlotItemID, const int32& NewAmount)
//{
//
//}

void UQuickSlotComponent::UpdateQuickSlot()
{
	for (const auto& MyItem : *GI->GetQuickSlotItems())
	{
		UE_LOG(LogTemp, Warning, TEXT("Broadcast %d"), MyItem.Value);

		//BroadCast to QuickSlot Widget
		if (QuickSlotUpdateDelegate.IsBound())
		{
			QuickSlotUpdateDelegate.Broadcast(MyItem.Key, MyItem.Value, *GI->GetInventoryItems()->Find(MyItem.Value));
		}
	}
}

void UQuickSlotComponent::ManageQuickSlot(const int32& TargetItemID, const int32& TargetItemAmount)
{
	UE_LOG(LogTemp, Error, TEXT("ManageQuickSlot"));

	// check item is already in quickslot
	// if true, Delete QuickSlot Item
	int32 ItemExistInQuickSlotIndex;
	if (IsItemExistInQuickSlot(TargetItemID, ItemExistInQuickSlotIndex))
	{
		DeleteItemFromQuickSlot(ItemExistInQuickSlotIndex);
		UGameplayStatics::PlaySound2D(GetWorld(), QuickSlotSound);

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
	SetItemToQuickSlot(QuickSlotIndex, TargetItemID, TargetItemAmount);
	UGameplayStatics::PlaySound2D(GetWorld(), QuickSlotSound);
}

void UQuickSlotComponent::UseQuickSlotItem()
{
	CHECK_VALID(GI);
	CHECK_VALID(PC);

	AFHPlayerCharacter* PlayerChar = PC->GetPawn<AFHPlayerCharacter>();
	CHECK_VALID(PlayerChar);

	if (PlayerChar->TempUseItemID == 0)
	{
		return;
	}
	int32 UseItemID = PlayerChar->TempUseItemID;
	PlayerChar->TempUseItemID = 0;

	FConsumableItemData QuickSlotConsumableItemData;
	GI->GetConsumableItemInfo(UItemDataManager::GetPureID(UseItemID), QuickSlotConsumableItemData);

	PlayerChar->C2S_PlayUseItemEffect(QuickSlotConsumableItemData.EffectTarget);

	UPlayerStatusComponent* PlayerStatusComp = PlayerChar->GetPlayerStatusComp();
	CHECK_VALID(PlayerStatusComp);

	switch (QuickSlotConsumableItemData.EffectTarget)
	{
	case EEffectTarget::Health:
		PlayerStatusComp->UpdateCurrentPlayerStats(QuickSlotConsumableItemData.EffectValue, 0);
		break;
	case EEffectTarget::Stamina:
		PlayerStatusComp->UpdateCurrentPlayerStats(0, QuickSlotConsumableItemData.EffectValue);
		break;
	case EEffectTarget::AttackPower:
	case EEffectTarget::AttackSpeed:
	case EEffectTarget::CriticalChance:
	case EEffectTarget::DefensivePower:
		UseTemporaryItem(QuickSlotConsumableItemData, PlayerStatusComp);
		break;
	default:
		break;
	}

	InventoryComp->RemoveItemFromInventory(UseItemID, 1);
}

void UQuickSlotComponent::SetItemToQuickSlot(const int32& NewQuickSlotIndex, const int32& NewItemID, const int32& NewItemAmount)
{
	// Check item already exist in quick slots
	// if true, delete exist item.
	int32 ItemExistInQuickSlotIndex;
	if (IsItemExistInQuickSlot(NewItemID, ItemExistInQuickSlotIndex))
	{
		DeleteItemFromQuickSlot(ItemExistInQuickSlotIndex);
	}
	
	// if Item Exist in NewQuickSlotIndex, delete item
	DeleteItemFromQuickSlot(NewQuickSlotIndex);

	GI->GetQuickSlotItems()->Add(NewQuickSlotIndex, NewItemID);

	//BroadCast to Inventory Widget
	if (InventoryComp->ItemRegisterDelegate.IsBound())
	{
		InventoryComp->ItemRegisterDelegate.Broadcast(NewItemID, true);
	}

	//BroadCast to QuickSlot Widget
	if (QuickSlotUpdateDelegate.IsBound())
	{
		QuickSlotUpdateDelegate.Broadcast(NewQuickSlotIndex, NewItemID, NewItemAmount);
	}

	//reserved
	CHECK_VALID(GI);
	UE_LOG(LogTemp, Warning, TEXT("QuickSlot OnItemRegister %d"), NewItemID);

	int32 RegistedItemID = NewItemID;
	UItemDataManager::RegistItem(RegistedItemID);

	*GI->GetQuickSlotItems()->Find(NewQuickSlotIndex) = RegistedItemID;
}

void UQuickSlotComponent::DeleteItemFromQuickSlot(const int32& NewQuickSlotIndex)
{
	if (!GI->GetQuickSlotItems()->Contains(NewQuickSlotIndex))
	{
		return;
	}

	int32 DeleteItemID = *GI->GetQuickSlotItems()->Find(NewQuickSlotIndex);

	GI->GetQuickSlotItems()->Remove(NewQuickSlotIndex);

	//BroadCast to Inventory Widget
	if (InventoryComp->ItemRegisterDelegate.IsBound())
	{
		InventoryComp->ItemRegisterDelegate.Broadcast(DeleteItemID, false);
		UE_LOG(LogTemp, Error, TEXT("ItemRegisterDelegate %d"), DeleteItemID);

	}

	//BroadCast to QuickSlot Widget
	if (QuickSlotUpdateDelegate.IsBound())
	{
		QuickSlotUpdateDelegate.Broadcast(NewQuickSlotIndex, DeleteItemID, 0);
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

bool UQuickSlotComponent::IsItemExistInQuickSlot(const int32& TargetItemID, int32& OutIndex)
{
	for (const auto& MyQuickslot : *GI->GetQuickSlotItems())
	{
		if (UItemDataManager::GetPureID(MyQuickslot.Value) == UItemDataManager::GetPureID(TargetItemID))
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

void UQuickSlotComponent::UseTemporaryItem(const FConsumableItemData& ConsumableItemData, UPlayerStatusComponent* PlayerStatusComp)
{
	switch (ConsumableItemData.EffectTarget)
	{
	case EEffectTarget::Health:
	case EEffectTarget::Stamina:
		break;
	case EEffectTarget::AttackPower:
		PlayerStatusComp->UpdateCurrentPlayerStats(0, 0, ConsumableItemData.EffectValue);
		break;
	case EEffectTarget::AttackSpeed:
		PlayerStatusComp->UpdateCurrentPlayerStats(0, 0, 0, ConsumableItemData.EffectValue);
		break;
	case EEffectTarget::CriticalChance:
		PlayerStatusComp->UpdateCurrentPlayerStats(0, 0, 0, 0, ConsumableItemData.EffectValue);
		break;
	case EEffectTarget::DefensivePower:
		PlayerStatusComp->UpdateCurrentPlayerStats(0, 0, 0, 0, 0, ConsumableItemData.EffectValue);
		break;
	default:
		break;
	}

	FTimerHandle RestoreStatsHandle;
	FTimerDelegate RestoreStatsDelegate;
	RestoreStatsDelegate.BindUFunction(this, TEXT("RestorePlayerStatus"), ConsumableItemData, PlayerStatusComp);
	GetWorld()->GetTimerManager().SetTimer(RestoreStatsHandle, RestoreStatsDelegate, ConsumableItemData.Duration, false);
}

void UQuickSlotComponent::RestorePlayerStatus(const FConsumableItemData& ConsumableItemData, UPlayerStatusComponent* PlayerStatusComp)
{
	switch (ConsumableItemData.EffectTarget)
	{
	case EEffectTarget::Health:
	case EEffectTarget::Stamina:
		break;
	case EEffectTarget::AttackPower:
		PlayerStatusComp->UpdateCurrentPlayerStats(0, 0, -ConsumableItemData.EffectValue);
		break;
	case EEffectTarget::AttackSpeed:
		PlayerStatusComp->UpdateCurrentPlayerStats(0, 0, 0, -ConsumableItemData.EffectValue);
		break;
	case EEffectTarget::CriticalChance:
		PlayerStatusComp->UpdateCurrentPlayerStats(0, 0, 0, 0, -ConsumableItemData.EffectValue);
		break;
	case EEffectTarget::DefensivePower:
		PlayerStatusComp->UpdateCurrentPlayerStats(0, 0, 0, 0, 0, -ConsumableItemData.EffectValue);
		break;
	default:
		break;
	}
}
