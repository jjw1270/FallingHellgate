// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataManager.h"
#include "Blueprint/UserWidget.h"
#include "ShopSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UShopSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_ItemName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_ItemPrice;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Item;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_BuyAmount;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_AmountUp;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_AmountDown;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Buy;

protected:
	UPROPERTY()
	class UFHGameInstance* GI;

	UPROPERTY()
	class UInventoryComponent* InventoryComp;

	UPROPERTY()
	class UShopWidget* ShopWidget;

	struct FConsumableItemData SloItemData;

	int32 BuyAmount;

	int32 Price;

protected:
	UFUNCTION()
	void AmountUp();

	UFUNCTION()
	void AmountDown();

	UFUNCTION()
	void Buy();

public:
	void SetSlotData(struct FConsumableItemData* NewItemData);

	void SetShopWidget(class UShopWidget* NewShopWidget);

};
