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

public:
	void SetNPCShopCharacter(class ANPCShopCharacter* NewNPCShopChar);

protected:
	UFUNCTION()
	void OnCloseShopClicked();

protected:
	void InitShopInventory();
};
