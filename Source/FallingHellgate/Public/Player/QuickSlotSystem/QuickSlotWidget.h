// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuickSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UQuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	void BindQuickSlotCompEvents();

protected:
	UPROPERTY()
	class UQuickSlotComponent* QuickSlotComp;

	FTimerHandle InitTimerHandle;

	UPROPERTY(BlueprintReadOnly, Category = Widget)
	class UHUDWidget* HUDWidget;

// Blueprint Bind Widgets
protected:
	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<class UQuickSlotSlotWidget> QuickSlotSlotClass;

	UPROPERTY()
	TArray<class UQuickSlotSlotWidget*> QuickSlotSlotArray;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* UIDragBtn;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* QuickSlotGrid;

protected:
	FVector2D MousePosOnDragStart;

	FTimerHandle DragTimerHandle;

protected:
	UFUNCTION()
	void OnQuickSlotUpdated(const int32& QuickSlotIndex, const int32& NewItemID, const int32& NewItemAmount);

//Drag Func
protected:
	UFUNCTION()
	void OnDragBtnPressed();

	UFUNCTION()
	void OnDragBtnReleased();
};
