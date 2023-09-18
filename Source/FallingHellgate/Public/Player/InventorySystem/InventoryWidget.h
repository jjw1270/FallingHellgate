// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	class UInventoryComponent* InventoryComp;

	FTimerHandle InitTimerHandle;

	UPROPERTY(BlueprintReadOnly, Category = Widget)
	class UHUDWidget* HUDWidget;

// Blueprint Bind Widgets
protected:
	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<class UInventorySlotWidget> InventorySlotClass;

	UPROPERTY(BlueprintReadOnly, Category = Widget)
	TArray<class UInventorySlotWidget*> InventorySlotArray;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* UIDragBtn;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* InventorySlotGrid;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* ItemInfoBox;

	UPROPERTY(meta = (BindWidget))
	UUserWidget* ItemTrash;

	UPROPERTY(meta = (BindWidget))
	class URemoveConfirmWidget* RemoveConfirmWidget;
	
protected:
	UPROPERTY(EditAnywhere, Category = Widget)
	int32 DefaultSlotGridRowRange;

	int32 SlotGridColRange{ 5 };

public:
	void AddNewItemToSlot(class UItemData* NewItemData, const int32& NewItemAmount);

protected:
	void CreateSlotWidgets(int32 Row);

	void BindInventoryCompEvents();

	UFUNCTION()
	void OnItemUpdated(class UItemData* UpdateItemData, const int32& UpdateAmount);

	UFUNCTION()
	void OnItemRegister(class UItemData* UpdateItemData, const bool& bIsRegist);

	UFUNCTION(BlueprintCallable, Category = Widget)
	void SortItemSlot();

//Drag Func
	UFUNCTION()
	void OnDragBtnPressed();

	UFUNCTION()
	void OnDragBtnReleased();

public:
	UFUNCTION(BlueprintCallable, Category = Widget)
	FORCEINLINE UUserWidget* GetItemInfoBox() const { return ItemInfoBox; }

	FORCEINLINE UUserWidget* GetItemTrash() const { return ItemTrash; }

	UFUNCTION(BlueprintCallable, Category = Widget)
	FORCEINLINE class URemoveConfirmWidget* GetRemoveConfirmWidget() const { return RemoveConfirmWidget; }

	UFUNCTION(BlueprintCallable, Category = Widget)
	FORCEINLINE TArray<class UInventorySlotWidget*> GetInventorySlotArray() const { return InventorySlotArray; }
};
