// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"
#include "FallingHellgate.h"
#include "NPCShopCharacter.h"
#include "Components/Button.h"
#include "FHGameInstance.h"
#include "ShopInventorySlotWidget.h"
#include "Components/ScrollBox.h"
#include "InventoryComponent.h"
#include "FHPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "HUDWidget.h"
#include "InventoryWidget.h"
#include "InventorySlotWidget.h"
#include "ItemDataManager.h"
#include "ShopSlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/BackgroundBlur.h"


void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_CloseShop->OnClicked.AddDynamic(this, &UShopWidget::OnCloseShopClicked);
	PC = Cast<AFHPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	CHECK_VALID(PC);
	PC->GetInventoryComp()->MoneyUpdateDelegate.AddDynamic(this, &UShopWidget::OnMoneyUpdate);
	PC->GetInventoryComp()->ItemUpdateDelegate.AddUObject(this, &UShopWidget::OnInventoryUpdate);

	GI = GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	TextBlock_MyMoney->SetText(FText::FromString(FString::FromInt(GI->GetPlayerMoney())));

	InitShopInventory();

	InitShopSlot();
}

void UShopWidget::SetNPCShopCharacter(ANPCShopCharacter* NewNPCShopChar)
{
	NPCShopChar = NewNPCShopChar;
}

void UShopWidget::ShowNoMoneyWidget()
{
	BackgroundBlur_NoMoney->SetVisibility(ESlateVisibility::Visible);
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

void UShopWidget::OnInventoryUpdate(const int32& NewItemID, const int32& NewAmount)
{
	//Refresh ShopInventorySlot
	for (const auto& InventorySlot : ShopInventorySlotArray)
	{
		int32 SlotItemID = InventorySlot->GetSlotItemID();
		if (SlotItemID == 0)
		{
			continue;
		}

		if (SlotItemID == NewItemID)
		{
			if (NewAmount == 0)
			{
				InventorySlot->RemoveFromParent();
				return;
			}
			InventorySlot->UpdateSlotAmount(NewAmount);
			return;
		}
	}

	AddShopInventorySlot(NewItemID, NewAmount);
}

void UShopWidget::InitShopInventory()
{
	CHECK_VALID(PC);
	UHUDWidget* HUDWidget = PC->GetHUDWidget();
	CHECK_VALID(HUDWidget);
	UInventoryWidget* InventoryWidget = HUDWidget->GetInventoryWidget();
	CHECK_VALID(InventoryWidget);

	CHECK_VALID(ShopInventorySlotClass);
	for (const auto& InventorySlot : InventoryWidget->GetInventorySlotArray())
	{
		if (InventorySlot->GetSlotItemID() == 0)
		{
			continue;
		}

		AddShopInventorySlot(InventorySlot->GetSlotItemID(), InventorySlot->GetSlotItemAmount());
	}
}

void UShopWidget::AddShopInventorySlot(const int32& NewItemID, int32 NewAmount)
{
	// create slot widget
	// set slot data = ItemName, price, amount, image, type
	// add slot to scrollbox

	// Dont create widget if item is on registed
	if (UItemDataManager::IsRegistered(NewItemID))
	{
		return;
	}

	UShopInventorySlotWidget* ShopInventorySlot = CreateWidget<UShopInventorySlotWidget>(GetWorld(), ShopInventorySlotClass);
	ShopInventorySlot->AddToViewport();
	CHECK_VALID(ShopInventorySlot);

	ShopInventorySlot->SetSlotData(NewItemID, NewAmount);
	ShopInventorySlotArray.Add(ShopInventorySlot);

	ScrollBox_Inventory->AddChild(ShopInventorySlot);
}

void UShopWidget::InitShopSlot()
{
	UDataTable* ConsumalbeItemDataTable = GI->GetConsumableItemDataTable();
	CHECK_VALID(ConsumalbeItemDataTable);
	
	TArray<FConsumableItemData*> AllConsumableItemData;
	ConsumalbeItemDataTable->GetAllRows<FConsumableItemData>(TEXT(""), AllConsumableItemData);

	CHECK_VALID(ShopSlotClass);
	int32 InRow = 0;
	int32 InColumn = 0;
	for (const auto& ConsumableItemData : AllConsumableItemData)
	{
		UShopSlotWidget* ShopSlot = AddShopSlot(ConsumableItemData);
		ShopSlot->SetShopWidget(this);
		UniformGridPanel_ShopItems->AddChildToUniformGrid(ShopSlot, InRow, InColumn);

		InColumn++;
		if (InColumn > 1)
		{
			InColumn = 0;
			InRow++;
		}
	}
}

UShopSlotWidget* UShopWidget::AddShopSlot(FConsumableItemData* NewItemData)
{
	UShopSlotWidget* ShopSlot = CreateWidget<UShopSlotWidget>(GetWorld(), ShopSlotClass);
	ShopSlot->AddToViewport();
	if (!ShopSlot)
	{
		return nullptr;
	}
	
	ShopSlot->SetSlotData(NewItemData);

	return ShopSlot;
}
