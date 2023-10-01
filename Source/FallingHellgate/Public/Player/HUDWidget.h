// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Widget)
	void SwichWidgetVisibility(class APlayerController* TargetPlayerController, const FName& WidgetName);
	
	UFUNCTION(BlueprintCallable, Category = Widget)
	void SetWidgetZOrderToTop(UUserWidget* TargetWidget);

	void WidgetDragStart(UUserWidget* TargetWidget);

	void DragWidget();

	void WidgetDragEnd();

	void CloseAllWidgets();

	void OpenBackgroundWidgets();

protected:
	bool CheckAllHandleableWidgetCollapsed();

protected:
	UPROPERTY()
	class UCanvasPanelSlot* DragTargetSlot;

	FVector2D MousePosOnDragStart;

	FTimerHandle DragTimerHandle;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = Widget)
	class UQuickSlotWidget* QuickSlotWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = Widget)
	class UInventoryWidget* InventoryWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = Widget)
	class UEquipmentWidget* EquipmentWidget;

protected:
	UUserWidget* GetUserwidgetFromFName(const FName& WidgetName);

public:
	UFUNCTION(BlueprintCallable, Category = Widget)
	FORCEINLINE class UQuickSlotWidget* GetQuickSlotWidget() const { return QuickSlotWidget; }

	UFUNCTION(BlueprintCallable, Category = Widget)
	FORCEINLINE class UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

	UFUNCTION(BlueprintCallable, Category = Widget)
	FORCEINLINE class UEquipmentWidget* GetEquipmentWidget() const { return EquipmentWidget; }

};
