// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FDele_Dynamic_StatusUpdate, int32, UpdateHealth, int32, UpdateStamina, int32, UpdateAttack, float, UpdateAttackSpeed, float, UpdateCritical, int32, UpdateDefence);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLINGHELLGATE_API UPlayerStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerStatusComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	class UFHGameInstance* GI;

protected:
	UFUNCTION()
	void OnWeaponUpdate(class UItemData* UpdateEquipItem, const bool& bIsEquip);

	UFUNCTION()
	void OnArmorUpdate(const EArmorType& UpdateArmorType, class UItemData* UpdateEquipItem, const bool& bIsEquip);

protected:
	UFUNCTION()
	void UpdateDefaultPlayerStats(const bool& bIsEquip, const int32& AddHealth, const int32& AddStamina, const int32& AddAttack, const float& AddAttackSpeed, const float& AddCritcal, const int32& AddDefence);

	void InitCurrentPlayerStats();

	void UpdateCurrentPlayerStats(const int32& AddHealth, const int32& AddStamina);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentHealth, Category = Status)
	int32 CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Status)
	int32 CurrentStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Status)
	int32 CurrentAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Status)
	float CurrentAttackSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Status)
	float CurrentCritcal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Status)
	int32 CurrentDefence;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Event)
	FDele_Dynamic_StatusUpdate StatusUpdateDelegate;

public:
	UFUNCTION()
	void OnRep_CurrentHealth();

};
