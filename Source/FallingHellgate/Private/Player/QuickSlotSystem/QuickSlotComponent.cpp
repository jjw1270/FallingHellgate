// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotSystem/QuickSlotComponent.h"
#include "FallingHellgate.h"
#include "ItemData.h"
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
}

void UQuickSlotComponent::ManageQuickSlot(UItemData* TargetItemData, const int32& TargetItemAmount)
{
	// check item is already in quickslot
	// if true, Delete QuickSlot Item
	int32 ItemExistInQuickSlotIndex;
	if (IsItemExistInQuickSlot(TargetItemData, ItemExistInQuickSlotIndex))
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
	SetItemToQuickSlot(QuickSlotIndex, TargetItemData, TargetItemAmount);
	UGameplayStatics::PlaySound2D(GetWorld(), QuickSlotSound);
}

void UQuickSlotComponent::UseQuickSlotItem(const int32& TargetQuickSlotIndex)
{
	UItemData* QuickSlotItemData = *GI->GetQuickSlotItems()->Find(TargetQuickSlotIndex);

	FConsumableItemData QuickSlotConsumableItemData;
	QuickSlotItemData->GetConsumableData(QuickSlotConsumableItemData);

	UE_LOG(LogTemp, Warning, TEXT("ItemData : %s"), *QuickSlotConsumableItemData.BaseData.Name);

	CHECK_VALID(PC);
	AFHPlayerCharacter* PlayerChar = PC->GetPawn<AFHPlayerCharacter>();
	CHECK_VALID(PlayerChar);
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

	InventoryComp->RemoveItemFromInventory(QuickSlotItemData, 1);
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
