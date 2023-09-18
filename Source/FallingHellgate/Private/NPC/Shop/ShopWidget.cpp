// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"
#include "FallingHellgate.h"
#include "NPCShopCharacter.h"
#include "Components/Button.h"
#include "FHGameInstance.h"
#include "ShopInventorySlotWidget.h"
#include "ItemData.h"
#include "Components/ScrollBox.h"
#include "InventoryComponent.h"
#include "FHPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "FHHUD.h"
#include "HUDWidget.h"
#include "InventoryWidget.h"
#include "InventorySlotWidget.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_CloseShop->OnClicked.AddDynamic(this, &UShopWidget::OnCloseShopClicked);
	PC = Cast<AFHPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	CHECK_VALID(PC);
	PC->GetInventoryComp()->MoneyUpdateDelegate.AddDynamic(this, &UShopWidget::OnMoneyUpdate);

	GI = GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	TextBlock_MyMoney->SetText(FText::FromString(FString::FromInt(GI->GetPlayerMoney())));

	InitShopInventory();
}

void UShopWidget::SetNPCShopCharacter(ANPCShopCharacter* NewNPCShopChar)
{
	NPCShopChar = NewNPCShopChar;
}

void UShopWidget::OnCloseShopClicked()
{
	NPCShopChar->CloseShop();
}

void UShopWidget::OnMoneyUpdate(int32 UpdateMoney)
{
	GI->GetPlayerMoney() += UpdateMoney;

	TextBlock_MyMoney->SetText(FText::FromString(FString::FromInt(GI->GetPlayerMoney())));
}

void UShopWidget::InitShopInventory()
{
	AFHHUD* Hud = PC->GetHUD<AFHHUD>();
	CHECK_VALID(Hud);
	UHUDWidget* HudWidget = Hud->GetHUDWidget();
	CHECK_VALID(HudWidget);
	UInventoryWidget* InventoryWidget = HudWidget->GetInventoryWidget();
	CHECK_VALID(InventoryWidget);

	CHECK_VALID(ShopInventorySlotClass);
	for (const auto& InventorySlot : InventoryWidget->GetInventorySlotArray())
	{
		if (!InventorySlot->GetSlotItemData())
		{
			continue;
		}

		AddShopInventorySlot(InventorySlot->GetSlotItemData(), InventorySlot->GetSlotItemAmount());
	}
}

void UShopWidget::AddShopInventorySlot(class UItemData* NewItemData, int32 NewAmount)
{
	// create slot widget
	// set slot data = ItemName, price, amount, image, type
	// add slot to scrollbox

	// Dont create widget if item is on registed
	if (NewItemData->IsRegisted())
	{
		return;
	}

	UShopInventorySlotWidget* ShopInventorySlot = CreateWidget<UShopInventorySlotWidget>(GetWorld(), ShopInventorySlotClass);
	ShopInventorySlot->AddToViewport();
	CHECK_VALID(ShopInventorySlot);

	ShopInventorySlot->SetSlotData(NewItemData, NewAmount);
	ShopInventorySlotArray.Add(ShopInventorySlot);

	ScrollBox_Inventory->AddChild(ShopInventorySlot);
}
