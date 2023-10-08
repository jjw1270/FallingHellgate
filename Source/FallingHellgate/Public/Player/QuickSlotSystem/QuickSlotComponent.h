// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuickSlotComponent.generated.h"


// Delegate called when an QuickSlot Item Changed
// const int32& QuickSlotIndex, UItemData* ItemData, const int32& ItemAmount
DECLARE_MULTICAST_DELEGATE_ThreeParams(FDele_Multi_QuickSlotUpdate, const int32&, const int32&, const int32&);

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
	class AFHPlayerController* PC;

	UPROPERTY()
	class UInventoryComponent* InventoryComp;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* QuickSlotSound;

// UI Delegate
public:
	FDele_Multi_QuickSlotUpdate QuickSlotUpdateDelegate;

public:
	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void ManageQuickSlot(const int32& TargetItemID, const int32& TargetItemAmount);

	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void UseQuickSlotItem();

	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void SetItemToQuickSlot(const int32& NewQuickSlotIndex, const int32& NewItemID, const int32& NewItemAmount);

	UFUNCTION(BlueprintCallable, Category = "Quick Slot")
	void DeleteItemFromQuickSlot(const int32& NewQuickSlotIndex);

	bool IsQuickSlotEmpty(int32 QuickSlotIndex);

protected:
	bool IsItemExistInQuickSlot(const int32& TargetItemID, int32& OutIndex);

	int32 GetEmptyQuickSlotSlotIndex();

	void UseTemporaryItem(const struct FConsumableItemData& ConsumableItemData, UPlayerStatusComponent* PlayerStatusComp);

	UFUNCTION()
	void RestorePlayerStatus(const struct FConsumableItemData& ConsumableItemData, UPlayerStatusComponent* PlayerStatusComp);
};
