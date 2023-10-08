// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RemoveConfirmWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API URemoveConfirmWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY()
	class UInventoryComponent* InventoryComp;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Data)
	int32 TargetItemID;

	UPROPERTY(BlueprintReadOnly, Category = Widget)
	int32 MaxAmount;

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Item;

	UPROPERTY(meta = (BindWidget))
	class USlider* Slider_Amount;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* SetAmountBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = Widget)
	class UTextBlock* Text_Amount;

public:
	UFUNCTION(BlueprintCallable, Category = Widget)
	void ShowRemoveConfirm(class UItemDragDropOperation* NewDragDropOperation);

protected:
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void RemoveItem();

	UFUNCTION(BlueprintCallable, Category = Widget)
	void OnCancel();

	void ClearWidget();
};
