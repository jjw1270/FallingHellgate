// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

DECLARE_DELEGATE(FDele_NoMoney);

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	class UFHGameInstance* GI;

	UPROPERTY()
	class AFHPlayerController* PC;

	UPROPERTY()
	class ANPCShopCharacter* NPCShopChar;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CloseShop;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_Inventory;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_MyMoney;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* UniformGridPanel_ShopItems;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UBackgroundBlur* BackgroundBlur_NoMoney;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UShopInventorySlotWidget> ShopInventorySlotClass;

	UPROPERTY()
	TArray<class UShopInventorySlotWidget*> ShopInventorySlotArray;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UShopSlotWidget> ShopSlotClass;

public:
	void SetNPCShopCharacter(class ANPCShopCharacter* NewNPCShopChar);

	void ShowNoMoneyWidget();

protected:
	UFUNCTION()
	void OnCloseShopClicked();

	UFUNCTION()
	void OnMoneyUpdate(int32 UpdateMoney);

	UFUNCTION()
	void OnInventoryUpdate(class UItemData* NewItemData, const int32& NewAmount);

protected:
	void InitShopInventory();

	void AddShopInventorySlot(class UItemData* NewItemData, int32 NewAmount);

	void InitShopSlot();

	class UShopSlotWidget* AddShopSlot(struct FConsumableItemData* NewItemData);
};
