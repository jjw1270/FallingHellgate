// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"
#include "FallingHellgate.h"
#include "NPCShopCharacter.h"
#include "Components/Button.h"
#include "FHGameInstance.h"
#include "ShopInventorySlotWidget.h"
#include "ItemData.h"
#include "Components/ScrollBox.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_CloseShop->OnClicked.AddDynamic(this, &UShopWidget::OnCloseShopClicked);

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

void UShopWidget::InitShopInventory()
{
	UFHGameInstance* GI = GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	CHECK_VALID(ShopInventorySlotClass);

	for (const auto& InventoryItem : *GI->GetInventoryItems())
	{
		AddShopInventorySlot(InventoryItem.Key, InventoryItem.Value);
	}
}

void UShopWidget::AddShopInventorySlot(class UItemData* NewItemData, int32 NewAmount)
{
	// create slot widget
	// set slot data = ItemName, price, amount, image, type
	// add slot to scrollbox
	UShopInventorySlotWidget* ShopInventorySlot = CreateWidget<UShopInventorySlotWidget>(GetWorld(), ShopInventorySlotClass);
	ShopInventorySlot->AddToViewport();
	CHECK_VALID(ShopInventorySlot);

	ShopInventorySlot->SetSlotData(NewItemData->GetBaseData(), NewItemData->GetItemType(), NewAmount);
	ShopInventorySlotArray.Add(ShopInventorySlot);

	ScrollBox_Inventory->AddChild(ShopInventorySlot);
}
