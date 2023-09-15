// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotSlotWidget.h"
#include "FallingHellgate.h"
#include "ItemData.h"
#include "InventoryComponent.h"
#include "QuickSlotComponent.h"
#include "FHPlayerController.h"
#include "ItemDragDropOperation.h"
#include "Components/Image.h"

void UQuickSlotSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AFHPlayerController* PC = Cast<AFHPlayerController>(GetOwningPlayer());
	CHECK_VALID(PC);

	InventoryComp = PC->GetInventoryComp();
	CHECK_VALID(InventoryComp);

	QuickSlotComp = PC->GetQuickSlotComp();
	CHECK_VALID(QuickSlotComp);

	// bind ItemUpdateDelegate
	InventoryComp->ItemUpdateDelegate.AddUObject(this, &UQuickSlotSlotWidget::OnUpdateItem);

	ClearSlot();
}

// Only for inventory slot to quick slot
bool UQuickSlotSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{	
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemDragDropOperation* DDOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DDOperation)
	{
		return false;
	}

	// Quick slot is only for Consumalbe Items
	if (!IsValid(DDOperation->DraggingItemData) && DDOperation->DraggingItemData->GetItemType() != EItemType::Consumable)
	{
		return false;
	}

	QuickSlotComp->SetItemToQuickSlot(Index, DDOperation->DraggingItemData, DDOperation->DraggingItemAmount); 

	return false;
}

FReply UQuickSlotSlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

	QuickSlotComp->DeleteItemFromQuickSlot(Index);

	return FReply::Handled();
}

void UQuickSlotSlotWidget::SetSlot(class UItemData* NewItemData, const int32& NewItemAmount)
{
	if (NewItemAmount <= 0)
	{
		ClearSlot();
		return;
	}

	SlotItemData = NewItemData;
	SlotItemAmount = NewItemAmount;

	ItemImageWidget->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	ItemImageWidget->SetBrushFromTexture(SlotItemData->GetBaseData().Icon2D);
}

void UQuickSlotSlotWidget::OnUpdateItem(class UItemData* UpdateItemData, const int32& UpdateAmount)
{
	if (IsEmpty())
	{
		return;
	}

	if (SlotItemData != UpdateItemData)
	{
		return;
	}

	SlotItemAmount = UpdateAmount;

	if (SlotItemAmount <= 0)
	{
		QuickSlotComp->DeleteItemFromQuickSlot(Index);
	}
}

bool UQuickSlotSlotWidget::IsEmpty()
{
	return !IsValid(SlotItemData);
}

void UQuickSlotSlotWidget::ClearSlot()
{
	SlotItemData = nullptr;
	SlotItemAmount = 0;

	ItemImageWidget->SetBrushFromTexture(nullptr);
	ItemImageWidget->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
}
