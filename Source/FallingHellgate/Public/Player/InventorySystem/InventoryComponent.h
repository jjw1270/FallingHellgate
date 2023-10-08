// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

// Delegate called when an inventory item changed
// const int32& ItemID, const int32& ItemAmount
DECLARE_MULTICAST_DELEGATE_TwoParams(FDele_Multi_ItemUpdate, const int32&, const int32&);

// Delegate called when inventory item is registered
// const int32& ItemID, const int64& UniqueID, const bool& bIsRegist
DECLARE_MULTICAST_DELEGATE_TwoParams(FDele_Multi_ItemRegister, const int32&, const bool&);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_MoneyUpdate, int32, UpdateMoney);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLINGHELLGATE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

protected:
	void InitComponent();

	UFUNCTION()
	void UpdateInventory();

protected:
	UPROPERTY()
	class UFHGameInstance* GI;

	UPROPERTY()
	class UEquipmentComponent* EquipComp;

	UPROPERTY()
	class UQuickSlotComponent* QuickSlotComp;

// UI delegate
public:
	FDele_Multi_ItemUpdate ItemUpdateDelegate;

	FDele_Multi_ItemRegister ItemRegisterDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Event)
	FDele_Dynamic_MoneyUpdate MoneyUpdateDelegate;

protected:
	UFUNCTION()
	void OnItemRegister(const int32& UpdateItemID, const bool& bIsRegist);

// Inventory func
public:
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddItemToInventory(const int32& NewPureItemID, const int32& NewAmount);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void RemoveItemFromInventory(const int32& TargetItemID, const int32& Amount);

	int32 MakeUniqueID(const int32& NewPureItemID);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void ManageItem(const int32& TargetItemID, const int32& TargetItemAmount);

};
