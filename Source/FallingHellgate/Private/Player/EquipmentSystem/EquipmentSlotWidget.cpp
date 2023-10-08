// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlotWidget.h"
#include "FallingHellgate.h"
#include "FHGameInstance.h"
#include "ItemData.h"
#include "Components/Image.h"
#include "EquipmentComponent.h"
#include "FHPlayerController.h"
#include "ItemDragDropOperation.h"

void UEquipmentSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Image_BackIcon->SetBrushFromTexture(BackIconTexture);
}

void UEquipmentSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GI = GetGameInstance<UFHGameInstance>();

	BindEquipmentCompEvents();
}

void UEquipmentSlotWidget::BindEquipmentCompEvents()
{
	AFHPlayerController* PC = GetOwningPlayer<AFHPlayerController>();

	if (IsValid(PC))
	{
		
		EquipComp = PC->GetEquipmentComp();
		if (IsValid(EquipComp))
		{
			if (ItemType == EItemType::Weapon)
			{
				EquipComp->WeaponUpdateDelegate.AddUObject(this, &UEquipmentSlotWidget::OnWeaponUpdate);
				return;
			}
			if (ItemType == EItemType::Armor)
			{
				EquipComp->ArmorUpdateDelegate.AddUObject(this, &UEquipmentSlotWidget::OnArmorUpdate);
				return;
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(InitTimerHandle, this, &UEquipmentSlotWidget::BindEquipmentCompEvents, 0.1f, false);
}

bool UEquipmentSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (!GI)
	{
		return false;
	}

	UItemDragDropOperation* DDOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DDOperation)
	{
		return false;
	}

	int32 DraggedItemID = DDOperation->DraggingItemID;
	if (DraggedItemID == 0)
	{
		return false;
	}

	// Equipment is only for Weapon/Armor Items
	if (GI->GetBaseItemData(DraggedItemID).Type != ItemType)
	{
		return false;
	}

	// Weapon
	if (ItemType == EItemType::Weapon)
	{
		EquipComp->ManageEquipment(DraggedItemID);

		return false;
	}

	// Armor
	FArmorItemData DraggedArmorItemData;
	GI->GetArmorItemInfo(DraggedItemID, DraggedArmorItemData);
	if (DraggedArmorItemData.ArmorType == ArmorType)
	{
		EquipComp->ManageEquipment(DraggedItemID);

		return false;
	}

	return false;
}

FReply UEquipmentSlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	if (IsEmpty())
	{
		return FReply::Unhandled();
	}

	//check left mouse double clicked
	TSet<FKey> PressedBtns = InMouseEvent.GetPressedButtons();
	for (const auto& pb : PressedBtns)
	{
		if (pb != EKeys::LeftMouseButton)
		{
			return FReply::Unhandled();
		}
	}

	EquipComp->ManageEquipment(EquippedItemID);

	return FReply::Handled();
}

void UEquipmentSlotWidget::OnWeaponUpdate(const int32& UpdateItemID, const bool& bEquip)
{
	// if UnEquip
	if (!bEquip)
	{
		ClearSlot();
		return;
	}

	SetSlot(UpdateItemID);
}

void UEquipmentSlotWidget::OnArmorUpdate(const EArmorType& UpdateArmorType, const int32& UpdateItemID, const bool& bEquip)
{
	//Check ArmorType
	if (ArmorType != UpdateArmorType)
	{
		return;
	}

	// UE_LOG(LogTemp, Warning, TEXT("OnArmorUpdate"));

	// if UnEquip
	if (!bEquip)
	{
		ClearSlot();
		return;
	}
	
	SetSlot(UpdateItemID);
}

void UEquipmentSlotWidget::SetSlot(const int32& UpdateItemID)
{
	CHECK_VALID(GI);

	EquippedItemID = UpdateItemID;

	FBaseItemData EquippedBaseItemData = GI->GetBaseItemData(EquippedItemID);

	Image_Equip->SetBrushFromTexture(EquippedBaseItemData.Icon2D);
	Image_Equip->SetVisibility(ESlateVisibility::Visible);
}

void UEquipmentSlotWidget::ClearSlot()
{
	EquippedItemID = 0;

	Image_Equip->SetBrushFromTexture(nullptr);
	Image_Equip->SetVisibility(ESlateVisibility::Collapsed);
}

bool UEquipmentSlotWidget::IsEmpty()
{
	return (EquippedItemID == 0) ? true : false;
}
