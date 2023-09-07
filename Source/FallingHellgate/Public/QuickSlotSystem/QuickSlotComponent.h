// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuickSlotComponent.generated.h"


// Delegate called when an QuickSlot Item Changed
// const int32& QuickSlotIndex, UItemData* ItemData, const int32& ItemAmount
DECLARE_MULTICAST_DELEGATE_ThreeParams(FDele_Multi_QuickSlotUpdate, const int32&, class UItemData*, const int32&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLINGHELLGATE_API UQuickSlotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UQuickSlotComponent();

protected:
	virtual void BeginPlay() override;

protected:
	void InitComponent();
		
protected:
	UPROPERTY()
	class UFHGameInstance* GI;

	UPROPERTY()
	class UInventoryComponent* InventoryComp;

// UI Delegate
public:
	FDele_Multi_QuickSlotUpdate QuickSlotUpdateDelegate;

public:
	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void ManageQuickSlot(UItemData* TargetItemData, const int32& TargetItemAmount);

	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void UseQuickSlotItem(const int32& TargetQuickSlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void SetItemToQuickSlot(const int32& NewQuickSlotIndex, class UItemData* NewItemData, const int32& NewItemAmount);

	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void DeleteItemFromQuickSlot(const int32& NewQuickSlotIndex);

protected:
	bool IsItemExistInQuickSlot(class UItemData* TargetItemData, int32& OutIndex);

	int32 GetEmptyQuickSlotSlotIndex();

};
