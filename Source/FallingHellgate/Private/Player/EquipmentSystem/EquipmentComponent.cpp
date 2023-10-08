// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "FallingHellgate.h"
#include "ItemDataManager.h"
#include "InventoryComponent.h"
#include "FHGameInstance.h"
#include "FHPlayerController.h"
#include "Kismet/GameplayStatics.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetComponentTickEnabled(false);
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	InitComponent();
}

void UEquipmentComponent::InitComponent()
{
	AFHPlayerController* PC = GetOwner<AFHPlayerController>();
	CHECK_VALID(PC);

	GI = PC->GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	InventoryComp = PC->GetInventoryComp();
	CHECK_VALID(InventoryComp);

	// InventoryComp->ItemRegisterDelegate.AddUObject(this, &UEquipmentComponent::OnItemRegister);

	FTimerHandle TempHandle;
	GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &UEquipmentComponent::UpdateEquipment, 1.f, false);
}

void UEquipmentComponent::UpdateEquipment()
{
	for (const auto& MyEquipID : *GI->GetEquipments())
	{
		UE_LOG(LogTemp, Warning, TEXT("Broadcast %d"), MyEquipID);

		// boradcast
		if (GI->GetBaseItemData(MyEquipID).Type == EItemType::Weapon)
		{
			if (WeaponUpdateDelegate.IsBound())
			{
				WeaponUpdateDelegate.Broadcast(MyEquipID, true);
			}

			return;
		}
		else if (GI->GetBaseItemData(MyEquipID).Type == EItemType::Armor)
		{
			FArmorItemData ArmorItemData;
			GI->GetArmorItemInfo(MyEquipID, ArmorItemData);

			if (ArmorUpdateDelegate.IsBound())
			{
				ArmorUpdateDelegate.Broadcast(ArmorItemData.ArmorType, MyEquipID, true);
			}

			return;
		}
	}
}

void UEquipmentComponent::ManageEquipment(const int32& TargetItemID)
{
	// check Item is already regist
	// if true, UnEquip target Item
	EItemType ExistItemType;
	EArmorType ExistArmorType;
	if (IsItemExistInEquipmentSlot(TargetItemID, ExistItemType, ExistArmorType))
	{
		UnEquip(TargetItemID);
		UGameplayStatics::PlaySound2D(GetWorld(), EquipmentSound);

		return;
	}

	// else, EquipItem
	Equip(TargetItemID);
	UGameplayStatics::PlaySound2D(GetWorld(), EquipmentSound);
}

void UEquipmentComponent::OnItemRegister(const int32& UpdateItemID, const bool& bIsRegist)
{
	CHECK_VALID(GI);

	UE_LOG(LogTemp, Warning, TEXT("Equip OnItemRegister %d"), UpdateItemID);

	GI->GetEquipments()->Remove(UpdateItemID);

	int32 RegistedItemID = UpdateItemID;
	UItemDataManager::RegistItem(RegistedItemID);

	GI->GetEquipments()->Add(RegistedItemID);
}

void UEquipmentComponent::Equip(const int32& NewItemID)
{
	CHECK_VALID(GI);
	
	EItemType NewItemType = UItemDataManager::GetItemType(NewItemID);
	FArmorItemData NewArmorData;
	GI->GetArmorItemInfo(UItemDataManager::GetPureID(NewItemID), NewArmorData);

	// Check Already Equipped at Same EquipType
	// if True : UnEquip Equipped Item
	for (auto& EquippedItemID : *GI->GetEquipments())
	{
		// Weapon ItemType
		if (NewItemType == EItemType::Weapon && UItemDataManager::GetItemType(EquippedItemID) == NewItemType)
		{
			UnEquip(EquippedItemID);
			break;
		}

		// Armor ItemType
		FArmorItemData EquippedArmorData;
		if (GI->GetArmorItemInfo(UItemDataManager::GetPureID(EquippedItemID), EquippedArmorData))
		{
			if (EquippedArmorData.ArmorType == NewArmorData.ArmorType)
			{
				UnEquip(EquippedItemID);
				break;
			}
		}
	}

	//Equip Item
	GI->GetEquipments()->Add(NewItemID);

	if (InventoryComp->ItemRegisterDelegate.IsBound())
	{
		InventoryComp->ItemRegisterDelegate.Broadcast(NewItemID, true);
		OnItemRegister(NewItemID, true);
	}

	// boradcast
	if (NewItemType == EItemType::Weapon)
	{
		if (WeaponUpdateDelegate.IsBound())
		{
			WeaponUpdateDelegate.Broadcast(NewItemID, true);
		}

		return;
	}
	else if (NewItemType == EItemType::Armor)
	{
		if (ArmorUpdateDelegate.IsBound())
		{
			ArmorUpdateDelegate.Broadcast(NewArmorData.ArmorType, NewItemID, true);
		}

		return;
	}
}

void UEquipmentComponent::UnEquip(const int32& TargetItemID)
{
	CHECK_VALID(GI);

	if (InventoryComp->ItemRegisterDelegate.IsBound())
	{
		InventoryComp->ItemRegisterDelegate.Broadcast(TargetItemID, false);
		OnItemRegister(TargetItemID, false);
	}

	EItemType TargetItemType = UItemDataManager::GetItemType(TargetItemID);

	if (TargetItemType == EItemType::Weapon)
	{
		// boradcast
		if (WeaponUpdateDelegate.IsBound())
		{
			WeaponUpdateDelegate.Broadcast(TargetItemID, false);
		}
	}
	else if (TargetItemType == EItemType::Armor)
	{
		FArmorItemData TargetArmorData;
		if (GI->GetArmorItemInfo(UItemDataManager::GetPureID(TargetItemID), TargetArmorData))
		{
			return;
		}

		// boradcast
		if (ArmorUpdateDelegate.IsBound())
		{
			ArmorUpdateDelegate.Broadcast(TargetArmorData.ArmorType, TargetItemID, false);
		}
	}

	GI->GetEquipments()->Remove(TargetItemID);
}

bool UEquipmentComponent::IsItemExistInEquipmentSlot(const int32& TargetItemID, EItemType& OutItemType, EArmorType& OutArmorType)
{
	if (!GI)
	{
		return false;
	}

	for (auto& MyEquipItemID : *GI->GetEquipments())
	{
		if (MyEquipItemID != TargetItemID)
		{
			continue;
		}

		OutItemType = UItemDataManager::GetItemType(TargetItemID);
		
		// Check Item has ArmorData
		FArmorItemData MyEquipArmorItemData;
		if (GI->GetArmorItemInfo(UItemDataManager::GetPureID(MyEquipItemID), MyEquipArmorItemData))
		{
			OutArmorType = MyEquipArmorItemData.ArmorType;
		}
		else
		{
			OutArmorType = EArmorType::None;
		}

		return true;
	}

	return false;
}
