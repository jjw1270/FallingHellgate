// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "FHPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API AFHPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFHPlayerController();

// Inherited Functions
protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

// Player Input
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* UIMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* MouseCursorVisibilityAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* InventoryUIAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* EquipmentUIAction;

// Input Func
protected:
	UFUNCTION(BlueprintCallable, Category = Widget)
	void SetMouseCursorVisibility();

	UFUNCTION(BlueprintCallable, Category = Widget)
	void WidgetOnOff(FName WidgetName);

// Components
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class UInventoryComponent* InventoryComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class UQuickSlotComponent* QuickSlotComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class UEquipmentComponent* EquipmentComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class UShopComponent* ShopComp;

public:
	UFUNCTION(BlueprintCallable)
	void CloseAllWidgets();

// Component Getter
public:
	UFUNCTION(BlueprintCallable, Category = Component)
	FORCEINLINE class UInventoryComponent* GetInventoryComp() const { return InventoryComp; }

	UFUNCTION(BlueprintCallable, Category = Component)
	FORCEINLINE class UQuickSlotComponent* GetQuickSlotComp() const { return QuickSlotComp; }

	UFUNCTION(BlueprintCallable, Category = Component)
	FORCEINLINE class UEquipmentComponent* GetEquipmentComp() const { return EquipmentComp; }

	UFUNCTION(BlueprintCallable, Category = Component)
	FORCEINLINE class UShopComponent* GetShopComp() const { return ShopComp; }

// UI
protected:
	UPROPERTY()
	class UHUDWidget* HUDWidget;

};
