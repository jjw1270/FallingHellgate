// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSlotWidget.h"
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

	UItemDragDropOperation* DDOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DDOperation)
	{
		return false;
	}

	UItemData* DraggedItemData = DDOperation->DraggingItemData;
	if (!DraggedItemData)
	{
		return false;
	}

	// Equipment is only for Weapon/Armor Items
	if (DraggedItemData->GetItemType() != ItemType)
	{
		return false;
	}

	// Weapon
	if (ItemType == EItemType::Weapon)
	{
		EquipComp->ManageEquipment(DraggedItemData);

		return false;
	}

	// Armor
	FArmorItemData DraggedArmorItemData;
	DraggedItemData->GetArmorData(DraggedArmorItemData);
	if (DraggedArmorItemData.ArmorType == ArmorType)
	{
		EquipComp->ManageEquipment(DraggedItemData);

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

	EquipComp->ManageEquipment(EquippedItemData);

	return FReply::Handled();
}

void UEquipmentSlotWidget::OnWeaponUpdate(class UItemData* UpdateItemData, const bool& bEquip)
{
	// if UnEquip
	if (!bEquip)
	{
		ClearSlot();
		return;
	}

	SetSlot(UpdateItemData);
}

void UEquipmentSlotWidget::OnArmorUpdate(const EArmorType& UpdateArmorType, UItemData* UpdateItemData, const bool& bEquip)
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
	
	SetSlot(UpdateItemData);
}

void UEquipmentSlotWidget::SetSlot(class UItemData* UpdateItemData)
{
	EquippedItemData = UpdateItemData;

	FBaseItemData EquippedBaseItemData = EquippedItemData->GetBaseData();

	Image_Equip->SetBrushFromTexture(EquippedBaseItemData.Icon2D);
	Image_Equip->SetVisibility(ESlateVisibility::Visible);
}

void UEquipmentSlotWidget::ClearSlot()
{
	EquippedItemData = nullptr;

	Image_Equip->SetBrushFromTexture(nullptr);
	Image_Equip->SetVisibility(ESlateVisibility::Collapsed);
}

bool UEquipmentSlotWidget::IsEmpty()
{
	return !IsValid(EquippedItemData);
}
