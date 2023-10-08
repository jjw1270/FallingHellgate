// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentWidget.generated.h"

/**
 * Equipment Widget doesnt take any equipment events, but equipment slot Widget Do.
 */
UCLASS()
class FALLINGHELLGATE_API UEquipmentWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

protected:
	void BindEquipmentCompEvents();

protected:
	FTimerHandle InitTimerHandle;

	UPROPERTY(BlueprintReadOnly, Category = Widget)
	class UHUDWidget* HUDWidget;

	// Blueprint Bind Widgets
protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* UIDragBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_ShowHelmet;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_ShowCloak;

//Drag Func
protected:
	UFUNCTION()
	void OnDragBtnPressed();

	UFUNCTION()
	void OnDragBtnReleased();

protected:
	UFUNCTION()
	void OnArmorUpdate(const EArmorType& UpdateArmorType, const int32& UpdateItemID, const bool& bEquip);

protected:
	UFUNCTION()
	void OnBtn_ShowHelmetClicked();

	UFUNCTION()
	void OnBtn_ShowCloakClicked();

protected:
	bool bIsHelmetShowed;

	bool bIsCloakShowed;
};
