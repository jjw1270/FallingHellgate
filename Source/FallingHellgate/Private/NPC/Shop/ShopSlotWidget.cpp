// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/Shop/ShopSlotWidget.h"
#include "FallingHellgate.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "FHPlayerController.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FHGameInstance.h"
#include "ShopWidget.h"

void UShopSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GI = GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	AFHPlayerController* PC = Cast<AFHPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	CHECK_VALID(PC);
	InventoryComp = PC->GetInventoryComp();
	CHECK_VALID(InventoryComp);
}

void UShopSlotWidget::AmountUp()
{
	++BuyAmount;

	TextBlock_BuyAmount->SetText(FText::FromString(FString::FromInt(BuyAmount)));
}

void UShopSlotWidget::AmountDown()
{
	--BuyAmount;
	if (BuyAmount < 1)
	{
		BuyAmount = 1;
	}

	TextBlock_BuyAmount->SetText(FText::FromString(FString::FromInt(BuyAmount)));
}

void UShopSlotWidget::Buy()
{
	int Pay = SloItemData.BaseData.BasePrice * BuyAmount;

	if (GI->GetPlayerMoney() < Pay)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Enough Money"));
		
		// show confirm widget
		CHECK_VALID(ShopWidget);
		ShopWidget->ShowNoMoneyWidget();

		BuyAmount = 1;
		TextBlock_BuyAmount->SetText(FText::FromString(FString::FromInt(BuyAmount)));

		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Buy Item %d"), Pay);

	// update money
	if (InventoryComp->MoneyUpdateDelegate.IsBound())
	{
		InventoryComp->MoneyUpdateDelegate.Broadcast(-Pay);
	}

	InventoryComp->AddItemToInventory(SloItemData.BaseData.ID, BuyAmount);

	BuyAmount = 1;
	TextBlock_BuyAmount->SetText(FText::FromString(FString::FromInt(BuyAmount)));
}

void UShopSlotWidget::SetSlotData(FConsumableItemData* NewItemData)
{
	SloItemData = *NewItemData;

	TextBlock_ItemName->SetText(FText::FromString(SloItemData.BaseData.Name));

	Price = SloItemData.BaseData.BasePrice * SloItemData.BaseData.UpgradeValue;
	TextBlock_ItemPrice->SetText(FText::FromString(FString::FromInt(Price)));

	Image_Item->SetBrushFromTexture(SloItemData.BaseData.Icon2D);

	BuyAmount = 1;

	Btn_AmountUp->OnClicked.AddDynamic(this, &UShopSlotWidget::AmountUp);
	Btn_AmountDown->OnClicked.AddDynamic(this, &UShopSlotWidget::AmountDown);
	Btn_Buy->OnClicked.AddDynamic(this, &UShopSlotWidget::Buy);
}

void UShopSlotWidget::SetShopWidget(UShopWidget* NewShopWidget)
{
	ShopWidget = NewShopWidget;
}
