// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickSlotSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UQuickSlotSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
// Native Func
protected:
	virtual void NativeConstruct() override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void SetSlot(class UItemData* NewItemData, const int32& NewItemAmount);

	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void ClearSlot();

protected:
	UFUNCTION()
	void OnUpdateItem(class UItemData* UpdateItemData, const int32& UpdateAmount);

protected:
	UPROPERTY(BlueprintReadOnly, Category = Component)
	class UInventoryComponent* InventoryComp;

	UPROPERTY(BlueprintReadOnly, Category = Component)
	class UQuickSlotComponent* QuickSlotComp;

	UPROPERTY(BlueprintReadOnly, Category = Widget)
	class UQuickSlotWidget* QuickSlotWidget;

// Variables to Bind UMG Components
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImageWidget;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Widget)
	int32 Index;

	UPROPERTY(BlueprintReadOnly, Category = Data)
	class UItemData* SlotItemData;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	int32 SlotItemAmount;

public:
	FORCEINLINE void SetIndex(const int32& NewIndex) { Index = NewIndex; }

	UFUNCTION(BlueprintCallable, Category = Widget)
	bool IsEmpty();
	
	UFUNCTION(BlueprintCallable, Category = Data)
	FORCEINLINE class UItemData* GetSlotItemData() const { return SlotItemData; }

	UFUNCTION(BlueprintCallable, Category = Inventory)
	FORCEINLINE int32 GetSlotItemAmount() const { return SlotItemAmount; }

};
