// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemDataManager.h"
#include "EquipmentComponent.generated.h"

/*
* <EquipmentItemID>
* Weapon : 2nnnn
* 
* <Armor>
* Helmet : 31nnn
* Upper  : 32nnn
* Gloves : 33nnn
* Lower  : 34nnn
* Shoes  : 35nnn
*/

// Delegate called when an Equipment Weapon Item Changed
// UItemData* ItemData, const bool& bEquip
DECLARE_MULTICAST_DELEGATE_TwoParams(FDele_Multi_WeaponUpdate, const int32&, const bool&);

// Delegate called when an Equipment Armor Item Changed
// const EArmorType& ArmorType, UItemData* ItemData, const bool& bEquip
DECLARE_MULTICAST_DELEGATE_ThreeParams(FDele_Multi_ArmorUpdate, const EArmorType&, const int32&, const bool&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLINGHELLGATE_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipmentComponent();

protected:
	virtual void BeginPlay() override;

protected:
	void InitComponent();

protected:
	UPROPERTY()
	class UFHGameInstance* GI;

	UPROPERTY()
	class UInventoryComponent* InventoryComp;

	UPROPERTY(EditDefaultsOnly)
	class USoundBase* EquipmentSound;

// UI Delegate
public:
	FDele_Multi_WeaponUpdate WeaponUpdateDelegate;

	FDele_Multi_ArmorUpdate ArmorUpdateDelegate;

public:
	UFUNCTION(BlueprintCallable, Category = Equipment)
	void ManageEquipment(const int32& TargetItemID);

protected:
	UFUNCTION()
	void OnItemRegister(const int32& UpdateItemID, const bool& bIsRegist);

protected:
	void Equip(const int32& NewItemID);

	void UnEquip(const int32& TargetItemID);

	bool IsItemExistInEquipmentSlot(const int32& TargetItemID, EItemType& OutItemType, EArmorType& OutArmorType);

protected:
	UFUNCTION()
	void UpdateEquipment();
};
