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
	class UFHGameInstance* GI;

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
	int32 EquippedItemID;

// Variables to Bind UMG Components
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_BackIcon;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Equip;

protected:
	UFUNCTION()
	void OnWeaponUpdate(const int32& UpdateItemID, const bool& bEquip);

	UFUNCTION()
	void OnArmorUpdate(const EArmorType& UpdateArmorType, const int32& UpdateItemID, const bool& bEquip);

public:
	UFUNCTION(BlueprintCallable, Category = Widget)
	void SetSlot(const int32& UpdateItemID);

	UFUNCTION(BlueprintCallable, Category = Widget)
	void ClearSlot();

	UFUNCTION(BlueprintCallable, Category = Widget)
	bool IsEmpty();
};
