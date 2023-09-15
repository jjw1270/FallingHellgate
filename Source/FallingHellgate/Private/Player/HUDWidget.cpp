// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "FallingHellgate.h"
#include "GameFramework/PlayerController.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

#include "QuickSlotWidget.h"
#include "InventoryWidget.h"
#include "EquipmentWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"

void UHUDWidget::SwichWidgetVisibility(class APlayerController* TargetPlayerController, const FName& WidgetName)
{
	UUserWidget* Widget = GetUserwidgetFromFName(WidgetName);
	CHECK_VALID(Widget);

	switch (Widget->GetVisibility())
	{
	//Open widget
	case ESlateVisibility::Collapsed:
		Widget->SetVisibility(ESlateVisibility::Visible);
		SetWidgetZOrderToTop(Widget);
		TargetPlayerController->SetShowMouseCursor(true);
		TargetPlayerController->SetInputMode(FInputModeGameAndUI());
		break;
	//Close widget
	case ESlateVisibility::Visible:
		Widget->SetVisibility(ESlateVisibility::Collapsed);
		if (CheckAllHandleableWidgetCollapsed())
		{
			TargetPlayerController->SetShowMouseCursor(false);
			TargetPlayerController->SetInputMode(FInputModeGameOnly());
		}
		break;
	default:
		break;
	}
}

void UHUDWidget::SetWidgetZOrderToTop(UUserWidget* TargetWidget)
{
	UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(GetRootWidget());
	CHECK_VALID(CanvasPanel);

	UCanvasPanelSlot* TargetWidgetSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);
	CHECK_VALID(TargetWidgetSlot);

	TargetWidgetSlot->SetZOrder(1);
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *Widget->GetName());


	for (auto PanelSlot : CanvasPanel->GetSlots())
	{
		if (PanelSlot == TargetWidgetSlot)
		{
			continue;
		}

		UCanvasPanelSlot* OtherSlot = Cast<UCanvasPanelSlot>(PanelSlot);
		CHECK_VALID(OtherSlot);

		OtherSlot->SetZOrder(0);
	}

}

void UHUDWidget::WidgetDragStart(UUserWidget* TargetWidget)
{
	DragTargetSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);
	if (!DragTargetSlot)
	{
		return;
	}

	MousePosOnDragStart = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()) - DragTargetSlot->GetPosition();

	GetWorld()->GetTimerManager().SetTimer(DragTimerHandle, this, &UHUDWidget::DragWidget, 0.01f, true);
}

void UHUDWidget::DragWidget()
{
	FVector2D WidgetPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()) - MousePosOnDragStart;


	DragTargetSlot->SetPosition(WidgetPos);
}

void UHUDWidget::WidgetDragEnd()
{
	if (DragTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DragTimerHandle);
	}
}

void UHUDWidget::CloseAllWidgets()
{
	QuickSlotWidget->SetVisibility(ESlateVisibility::Collapsed);
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	EquipmentWidget->SetVisibility(ESlateVisibility::Collapsed);
}

bool UHUDWidget::CheckAllHandleableWidgetCollapsed()
{
	if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		return false;
	}
	if (EquipmentWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		return false;
	}

	return true;
}

UUserWidget* UHUDWidget::GetUserwidgetFromFName(const FName& WidgetName)
{
	if (WidgetName == QuickSlotWidget->GetFName())
	{
		return QuickSlotWidget;
	}
	if(WidgetName == InventoryWidget->GetFName())
	{
		return InventoryWidget;
	}
	if (WidgetName == EquipmentWidget->GetFName())
	{
		return EquipmentWidget;
	}

	return nullptr;
}
