// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "FallingHellgate.h"
#include "ItemDataManager.h"

#include "InventoryComponent.h"
#include "FHPlayerController.h"
// Widget Components
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "InventorySlotWidget.h"

#include "HUDWidget.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateSlotWidgets(DefaultSlotGridRowRange);

	// Bind UI Drag Event
	UIDragBtn->OnPressed.AddDynamic(this, &UInventoryWidget::OnDragBtnPressed);
	UIDragBtn->OnReleased.AddDynamic(this, &UInventoryWidget::OnDragBtnReleased);

	// Bind InventoryComponent Delegates
	BindInventoryCompEvents();
}

void UInventoryWidget::CreateSlotWidgets(int32 Row)
{
	CHECK_VALID(InventorySlotClass);

	for (int32 row = 0; row < Row; row++)
	{
		for (int32 col = 0; col < SlotGridColRange; col++)
		{
			UInventorySlotWidget* NewInventorySlot = Cast<UInventorySlotWidget>(CreateWidget(GetOwningPlayer(), InventorySlotClass));
			NewInventorySlot->SetOwningInventoryWidget(this);

			InventorySlotGrid->AddChildToUniformGrid(NewInventorySlot, row, col);
			InventorySlotArray.Add(NewInventorySlot);
		}
	}
}

void UInventoryWidget::BindInventoryCompEvents()
{
	AFHPlayerController* PC = GetOwningPlayer<AFHPlayerController>();

	if (IsValid(PC))
	{
		InventoryComp = PC->GetInventoryComp();
		if (IsValid(InventoryComp))
		{
			InventoryComp->ItemUpdateDelegate.AddUObject(this, &UInventoryWidget::OnItemUpdated);
			InventoryComp->ItemRegisterDelegate.AddUObject(this, &UInventoryWidget::OnItemRegister);

			return;
		}
	}

	GetWorld()->GetTimerManager().SetTimer(InitTimerHandle, this, &UInventoryWidget::BindInventoryCompEvents, 0.1f, false);
}

void UInventoryWidget::OnItemUpdated(const int32& UpdateItemID, const int32& UpdateAmount)
{
	// Check Item already exist in slot, stack item
	// If Item is NonStackable, Item will be AddNewItemToSlot cause of UniqueID
	for (auto& InventorySlot : InventorySlotArray)
	{
		if (UItemDataManager::GetPureID(InventorySlot->GetSlotItemID()) == UItemDataManager::GetPureID(UpdateItemID))
		{
			InventorySlot->SetSlot(UpdateItemID, UpdateAmount);

			return;
		}
	}

	// else Add Item to New Slot
	AddNewItemToSlot(UpdateItemID, UpdateAmount);
}

void UInventoryWidget::AddNewItemToSlot(const int32& NewItemID, const int32& NewItemAmount)
{
	for (auto& slot : InventorySlotArray)
	{
		if (slot->IsEmpty())
		{
			slot->SetSlot(NewItemID, NewItemAmount);
			return;
		}
	}

	//If All Slots are Full, Make New Slots On New Row
	CreateSlotWidgets(1);
	AddNewItemToSlot(NewItemID, NewItemAmount);
}

void UInventoryWidget::OnItemRegister(const int32& UpdateItemID, const bool& bIsRegist)
{
	// Set Register Image visibility
	for (auto& InventorySlot : InventorySlotArray)
	{
		if (InventorySlot->GetSlotItemID() <= 0)
		{
			continue;
		}

		if (UItemDataManager::GetPureID(InventorySlot->GetSlotItemID()) == UItemDataManager::GetPureID(UpdateItemID))
		{
			UE_LOG(LogTemp, Warning, TEXT("InventorySlot->GetSlotItemID() == UpdateItemID"));
			InventorySlot->SetOnRegistImageVisibility(bIsRegist);
			return;
		}
	}

	// for Item in Inventory is dragged to QuickSlot slot
	GetWorld()->GetTimerManager().SetTimerForNextTick([this, UpdateItemID, bIsRegist]()
		{
			OnItemRegister(UpdateItemID, bIsRegist);
		});
}

// Sort according to IsEmpty()
void UInventoryWidget::SortItemSlot()
{
	for (int i = 0; i < InventorySlotArray.Num(); i++)
	{
		if (!InventorySlotArray[i]->IsEmpty())
		{
			continue;
		}

		for (int j = InventorySlotArray.Num()-1; j > i; j--)
		{
			if (!InventorySlotArray[j]->IsEmpty())
			{
				InventorySlotArray[i]->SetSlot(InventorySlotArray[j]->GetSlotItemID(), InventorySlotArray[j]->GetSlotItemAmount());
				InventorySlotArray[i]->SetOnRegistImageVisibility(InventorySlotArray[j]->IsSlotItemRegisted());

				InventorySlotArray[j]->ClearSlot();
				break;
			}
		}
	}
}

void UInventoryWidget::OnDragBtnPressed()
{
	
	if (!HUDWidget)
	{
		AFHPlayerController* PC = GetOwningPlayer<AFHPlayerController>();
		CHECK_VALID(PC);
		HUDWidget = PC->GetHUDWidget();
	}

	HUDWidget->WidgetDragStart(this);
}

void UInventoryWidget::OnDragBtnReleased()
{
	CHECK_VALID(HUDWidget);

	HUDWidget->WidgetDragEnd();
}
