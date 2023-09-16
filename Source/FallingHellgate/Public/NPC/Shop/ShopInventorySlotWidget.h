// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Item;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_ItemName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_ItemPrice;

public:
	// void SetItemImage(class )
};
