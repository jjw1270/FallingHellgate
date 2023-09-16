// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"
#include "FallingHellgate.h"
#include "NPCShopCharacter.h"
#include "Components/Button.h"
#include "FHGameInstance.h"

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

	for (const auto& InventoryItem : *GI->GetInventoryItems())
	{
		// create slot widget
		// set slot data = ItemName, price, amount, image
		// add slot to scrollbox
	}
}
