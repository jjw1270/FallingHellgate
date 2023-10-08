// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoveConfirmWidget.h"
#include "FHGameInstance.h"
#include "FallingHellgate.h"
//Widget Comp
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

#include "ItemDragDropOperation.h"
#include "FHPlayerController.h"
#include "InventoryComponent.h"

void URemoveConfirmWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AFHPlayerController* PC = Cast<AFHPlayerController>(GetOwningPlayer());
	CHECK_VALID(PC);

	InventoryComp = PC->GetInventoryComp();
	CHECK_VALID(InventoryComp);

	ClearWidget();
}

void URemoveConfirmWidget::ShowRemoveConfirm(UItemDragDropOperation* NewDragDropOperation)
{
	UFHGameInstance* GI = GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	TargetItemID = NewDragDropOperation->DraggingItemID;
	MaxAmount = NewDragDropOperation->DraggingItemAmount;

	Slider_Amount->SetMaxValue(MaxAmount);

	Image_Item->SetBrushFromTexture(GI->GetBaseItemData(TargetItemID).Icon2D);

	if (MaxAmount == 1)
	{
		SetAmountBox->SetVisibility(ESlateVisibility::Collapsed);
	}

	GetParent()->SetVisibility(ESlateVisibility::Visible);
}

void URemoveConfirmWidget::RemoveItem()
{
	int32 Amount = Slider_Amount->GetValue();

	InventoryComp->RemoveItemFromInventory(TargetItemID, Amount);

	OnCancel();
}

void URemoveConfirmWidget::OnCancel()
{
	SetAmountBox->SetVisibility(ESlateVisibility::Visible);
	GetParent()->SetVisibility(ESlateVisibility::Collapsed);

	ClearWidget();
}

void URemoveConfirmWidget::ClearWidget()
{
	TargetItemID = 0;
	MaxAmount = 0;

	Slider_Amount->SetValue(1.f);

	Text_Amount->SetText(FText::FromString(TEXT("1")));
}
