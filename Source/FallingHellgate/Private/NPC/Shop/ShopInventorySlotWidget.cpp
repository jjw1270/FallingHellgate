// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Shop/ShopInventorySlotWidget.h"
#include "FallingHellgate.h"
#include "Kismet/GameplayStatics.h"
#include "FHPlayerController.h"
#include "InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "FHGameInstance.h"

void UShopInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AFHPlayerController* PC = Cast<AFHPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	CHECK_VALID(PC);
	InventoryComp = PC->GetInventoryComp();
	CHECK_VALID(InventoryComp);
}

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
	// update money
	if (InventoryComp->MoneyUpdateDelegate.IsBound())
	{
		InventoryComp->MoneyUpdateDelegate.Broadcast(Price * SellAmount);
	}
	// remove inventory item from inventory comp
	InventoryComp->RemoveItemFromInventory(SloItemID, SellAmount);
}

void UShopInventorySlotWidget::SetSlotData(const int32& NewItemID, const int32& NewAmount)
{
	UFHGameInstance* GI = GetWorld()->GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	SloItemID = NewItemID;

	FBaseItemData SlotBaseItemData = GI->GetBaseItemData(SloItemID);

	TextBlock_ItemName->SetText(FText::FromString(SlotBaseItemData.Name));

	FString ItemTypeText = UEnum::GetValueAsString(GI->GetBaseItemData(SloItemID).Type);
	ItemTypeText.Split(TEXT("::"), nullptr, &ItemTypeText, ESearchCase::CaseSensitive, ESearchDir::FromEnd);
	TextBlock_ItemType->SetText(FText::FromString(ItemTypeText));

	Price = SlotBaseItemData.BasePrice * SlotBaseItemData.UpgradeValue;
	TextBlock_ItemPrice->SetText(FText::FromString(FString::FromInt(Price)));

	Image_Item->SetBrushFromTexture(SlotBaseItemData.Icon2D);

	MaxAmount = NewAmount;
	SellAmount = 1;

	Btn_AmountUp->OnClicked.AddDynamic(this, &UShopInventorySlotWidget::AmountUp);
	Btn_AmountDown->OnClicked.AddDynamic(this, &UShopInventorySlotWidget::AmountDown);
	Btn_Sell->OnClicked.AddDynamic(this, &UShopInventorySlotWidget::Sell);
}

void UShopInventorySlotWidget::UpdateSlotAmount(const int32& NewAmount)
{
	MaxAmount = NewAmount;
	SellAmount = 1;
	TextBlock_SellAmount->SetText(FText::FromString(FString::FromInt(SellAmount)));
}
