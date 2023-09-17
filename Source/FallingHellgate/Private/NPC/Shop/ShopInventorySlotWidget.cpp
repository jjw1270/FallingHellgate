// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Shop/ShopInventorySlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UShopInventorySlotWidget::AmountUp()
{
	++SellAmount;
	if (SellAmount > MaxAmount)
	{
		SellAmount = MaxAmount;
	}

	TextBlock_SellAmount->SetText(FText::FromString(FString::FromInt(SellAmount)));
}

void UShopInventorySlotWidget::AmountDown()
{
	--SellAmount;
	if (SellAmount < 1)
	{
		SellAmount = 1;
	}

	TextBlock_SellAmount->SetText(FText::FromString(FString::FromInt(SellAmount)));
}

void UShopInventorySlotWidget::Sell()
{
	UE_LOG(LogTemp, Warning, TEXT("CONFIRM SELL"));
}

void UShopInventorySlotWidget::SetSlotData(const FBaseItemData& NewBaseItemData, const EItemType& NewItemType, const int32& NewAmount)
{
	TextBlock_ItemName->SetText(FText::FromString(NewBaseItemData.Name));

	FString ItemTypeText = UEnum::GetValueAsString(NewItemType);
	ItemTypeText.Split(TEXT("::"), nullptr, &ItemTypeText, ESearchCase::CaseSensitive, ESearchDir::FromEnd);
	TextBlock_ItemType->SetText(FText::FromString(ItemTypeText));

	int32 Price = NewBaseItemData.BasePrice * NewBaseItemData.UpgradeValue;
	TextBlock_ItemPrice->SetText(FText::FromString(FString::FromInt(Price)));

	Image_Item->SetBrushFromTexture(NewBaseItemData.Icon2D);

	MaxAmount = NewAmount;
	SellAmount = 1;

	Btn_AmountUp->OnClicked.AddDynamic(this, &UShopInventorySlotWidget::AmountUp);
	Btn_AmountDown->OnClicked.AddDynamic(this, &UShopInventorySlotWidget::AmountDown);
	Btn_Sell->OnClicked.AddDynamic(this, &UShopInventorySlotWidget::Sell);
}
