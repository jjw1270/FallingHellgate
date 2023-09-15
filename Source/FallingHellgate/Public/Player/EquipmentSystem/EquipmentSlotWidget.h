// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDataManager.h"
#include "EquipmentSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	void BindEquipmentCompEvents();

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	UPROPERTY()
	class UEquipmentComponent* EquipComp;

	FTimerHandle InitTimerHandle;

//Need Edit Vars
protected:
	UPROPERTY(EditAnywhere, Category = Widget)
	UTexture2D* BackIconTexture;

	UPROPERTY(EditAnywhere, Category = Widget)
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere, Category = Widget)
	EArmorType ArmorType;

protected:
	UPROPERTY()
	class UItemData* EquippedItemData;

// Variables to Bind UMG Components
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_BackIcon;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Equip;

protected:
	UFUNCTION()
	void OnWeaponUpdate(class UItemData* UpdateItemData, const bool& bEquip);

	UFUNCTION()
	void OnArmorUpdate(const EArmorType& UpdateArmorType, UItemData* UpdateItemData, const bool& bEquip);

public:
	UFUNCTION(BlueprintCallable, Category = Widget)
	void SetSlot(class UItemData* UpdateItemData);

	UFUNCTION(BlueprintCallable, Category = Widget)
	void ClearSlot();

	UFUNCTION(BlueprintCallable, Category = Widget)
	bool IsEmpty();
};
