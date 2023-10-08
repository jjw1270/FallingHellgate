// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Blueprint/UserWidget.h"
#include "ShopInventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UShopInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_ItemName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_ItemType;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_ItemPrice;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Item;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_SellAmount;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_AmountUp;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_AmountDown;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Sell;

protected:
	UPROPERTY()
	class UInventoryComponent* InventoryComp;

	UPROPERTY()
	int32 SloItemID;

	int32 MaxAmount;

	int32 SellAmount;

	int32 Price;

protected:
	UFUNCTION()
	void AmountUp();

	UFUNCTION()
	void AmountDown();

	UFUNCTION()
	void Sell();

public:
	void SetSlotData(const int32& NewItemID, const int32& NewAmount);

	void UpdateSlotAmount(const int32& NewAmount);

	FORCEINLINE const int32& GetSlotItemID() const { return SloItemID; }
};
