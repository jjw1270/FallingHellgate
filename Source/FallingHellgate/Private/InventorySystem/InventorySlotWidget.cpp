// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlotWidget.h"
#include "FallingHellgate.h"
#include "ItemData.h"
// Drag Drop Operation
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ItemDragDropOperation.h"
#include "OnDragWidget.h"

#include "InventoryComponent.h"
#include "InventoryWidget.h"

#include "Components/Image.h"

void UInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InventoryComp = Cast<UInventoryComponent>(GetOwningPlayer()->GetComponentByClass(UInventoryComponent::StaticClass()));
	ensureMsgf(InventoryComp, TEXT("InventoryComp is nullptr"));

	ClearSlot();
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return reply.NativeReply;
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (IsEmpty())
	{
		return;
	}

	CHECK_VALID(DragWidgetClass);
	UOnDragWidget* DragWidget = Cast<UOnDragWidget>(CreateWidget(GetOwningPlayer(), DragWidgetClass));

	FBaseItemData BaseItemData = SlotItemData->GetBaseData();

	DragWidget->SetItemImage(BaseItemData.Icon2D);

	UItemDragDropOperation* DDOperation = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemDragDropOperation::StaticClass()));
	CHECK_VALID(DDOperation);

	DDOperation->DefaultDragVisual = DragWidget;
	DDOperation->DraggingItemData = SlotItemData;
	DDOperation->DraggingItemAmount = SlotItemAmount;
	if (IsSlotItemRegisted())
	{
		DDOperation->bIsRegist = true;
	}
	
	DDOperation->Payload = this;

	// Not Delete Item from Inventory, just Clear in Widget
	ClearSlot();

	InventoryWidget->GetItemInfoBox()->SetVisibility(ESlateVisibility::Collapsed);
	if (!DDOperation->bIsRegist)
	{
		InventoryWidget->GetItemTrash()->SetVisibility(ESlateVisibility::Visible);
	}

	OutOperation = DDOperation;
}

void UInventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	InventoryWidget->GetItemTrash()->SetVisibility(ESlateVisibility::Collapsed);

	UItemDragDropOperation* DDOperation = Cast<UItemDragDropOperation>(InOperation);
	CHECK_VALID(DDOperation);

	// If a new item is added to the PrevInventorySlot, Add to New slot
	if (!IsEmpty())
	{
		InventoryWidget->AddNewItemToSlot(DDOperation->DraggingItemData, DDOperation->DraggingItemAmount);
		return;
	}

	SetSlot(DDOperation->DraggingItemData, DDOperation->DraggingItemAmount);
}

// Called On Drag Droped Slot!
bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	InventoryWidget->GetItemTrash()->SetVisibility(ESlateVisibility::Collapsed);

	UItemDragDropOperation* DDOperation = Cast<UItemDragDropOperation>(InOperation);
	if (!DDOperation)
	{
		return false;
	}

	// if slot is empty
	if (IsEmpty())
	{	
		SetSlot(DDOperation->DraggingItemData, DDOperation->DraggingItemAmount);
		SetOnRegistImageVisibility(DDOperation->bIsRegist);

		return true;
	}

	// else slot has Item, Swap slots
	UInventorySlotWidget* PrevInventorySlot = Cast<UInventorySlotWidget>(DDOperation->Payload);
	if (!PrevInventorySlot)
	{
		return false;
	}

	PrevInventorySlot->SetSlot(SlotItemData, SlotItemAmount);
	if (IsSlotItemRegisted())
	{
		PrevInventorySlot->SetOnRegistImageVisibility(true);
	}
	
	ClearSlot();
	SetSlot(DDOperation->DraggingItemData, DDOperation->DraggingItemAmount);
	SetOnRegistImageVisibility(DDOperation->bIsRegist);

	return true;
}

FReply UInventorySlotWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

	InventoryComp->ManageItem(SlotItemData, SlotItemAmount);

	return FReply::Handled();
}

void UInventorySlotWidget::SetOwningInventoryWidget(UInventoryWidget* NewInventoryWidget)
{
	InventoryWidget = NewInventoryWidget;
}

void UInventorySlotWidget::SetSlot(class UItemData* NewItemData, const int32& NewAmount)
{
	if (NewAmount <= 0)
	{
		ClearSlot();
		return;
	}

	if (!SlotItemData)
	{
		Image_Item->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}

	SlotItemData = NewItemData;
	SlotItemAmount = NewAmount;

	Image_Item->SetBrushFromTexture(SlotItemData->GetBaseData().Icon2D);

	// If InfoBox is on Visible, Set Collapsed
	if (InventoryWidget)
	{
		UUserWidget* ItemInfoBox = InventoryWidget->GetItemInfoBox();
		if (ItemInfoBox->GetVisibility() == ESlateVisibility::Visible)
		{
			ItemInfoBox->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UInventorySlotWidget::ClearSlot()
{
	SlotItemData = nullptr;
	SlotItemAmount = 0;

	Image_Item->SetBrushFromTexture(nullptr);
	Image_Item->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));

	SetOnRegistImageVisibility(false);
}

void UInventorySlotWidget::SetOnRegistImageVisibility(const bool& bIsRegist)
{
	if (bIsRegist)
	{
		Image_OnRegist->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Image_OnRegist->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UInventorySlotWidget::IsSlotItemRegisted()
{
	if (Image_OnRegist->GetVisibility() == ESlateVisibility::Visible)
	{
		return true;
	}

	return false;
}

bool UInventorySlotWidget::IsEmpty()
{
	return !IsValid(SlotItemData);
}
