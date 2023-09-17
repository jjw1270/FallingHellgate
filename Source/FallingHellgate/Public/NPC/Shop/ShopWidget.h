// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	class ANPCShopCharacter* NPCShopChar;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CloseShop;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_Inventory;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_MyMoney;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UShopInventorySlotWidget> ShopInventorySlotClass;

	UPROPERTY()
	TArray<class UShopInventorySlotWidget*> ShopInventorySlotArray;

public:
	void SetNPCShopCharacter(class ANPCShopCharacter* NewNPCShopChar);

protected:
	UFUNCTION()
	void OnCloseShopClicked();

protected:
	void InitShopInventory();

	void AddShopInventorySlot(class UItemData* NewItemData, int32 NewAmount);

	
};
