// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotWidget.h"
#include "FallingHellgate.h"
#include "FHGameInstance.h"
#include "FHPlayerController.h"
#include "QuickSlotComponent.h"
#include "QuickSlotSlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "HUDWidget.h"

void UQuickSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Create QuickSlot Slots
	CHECK_VALID(QuickSlotSlotClass);
	for (int32 col = 0; col < 6; col++)
	{
		UQuickSlotSlotWidget* NewQuickSlotSlot = Cast<UQuickSlotSlotWidget>(CreateWidget(GetOwningPlayer(), QuickSlotSlotClass));
		NewQuickSlotSlot->SetIndex(col);

		QuickSlotGrid->AddChildToUniformGrid(NewQuickSlotSlot, 0, col);
		QuickSlotSlotArray.Add(NewQuickSlotSlot);
	}

	// Bind UI Drag Event
	UIDragBtn->OnPressed.AddDynamic(this, &UQuickSlotWidget::OnDragBtnPressed);
	UIDragBtn->OnReleased.AddDynamic(this, &UQuickSlotWidget::OnDragBtnReleased);

	// Bind QuickSlotComponent Delegates
	BindQuickSlotCompEvents();
}

void UQuickSlotWidget::BindQuickSlotCompEvents()
{
	AFHPlayerController* PC = GetOwningPlayer<AFHPlayerController>();

	if (IsValid(PC))
	{
		QuickSlotComp = PC->GetQuickSlotComp();
		if (IsValid(QuickSlotComp))
		{
			QuickSlotComp->QuickSlotUpdateDelegate.AddUObject(this, &UQuickSlotWidget::OnQuickSlotUpdated);

			return;
		}
	}

	GetWorld()->GetTimerManager().SetTimer(InitTimerHandle, this, &UQuickSlotWidget::BindQuickSlotCompEvents, 0.1f, false);
}

void UQuickSlotWidget::OnQuickSlotUpdated(const int32& QuickSlotIndex, const int32& NewItemID, const int32& NewItemAmount)
{
	UQuickSlotSlotWidget* QuickSlotSlot = QuickSlotSlotArray[QuickSlotIndex];

	QuickSlotSlot->SetSlot(NewItemID, NewItemAmount);
}

void UQuickSlotWidget::OnDragBtnPressed()
{
	if (!HUDWidget)
	{
		AFHPlayerController* PC = GetOwningPlayer<AFHPlayerController>();
		CHECK_VALID(PC);
		HUDWidget = PC->GetHUDWidget();
	}

	HUDWidget->WidgetDragStart(this);
}

void UQuickSlotWidget::OnDragBtnReleased()
{
	CHECK_VALID(HUDWidget);

	HUDWidget->WidgetDragEnd();
}
