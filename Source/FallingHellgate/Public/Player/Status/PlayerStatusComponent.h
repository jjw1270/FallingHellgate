// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FDele_Dynamic_DefaultStatusUpdate, int32, UpdateHealth, int32, UpdateStamina, int32, UpdateAttack, float, UpdateAttackSpeed, float, UpdateCritical, int32, UpdateDefence);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FDele_Dynamic_CurrentStatusUpdate, int32, UpdateHealth, int32, UpdateStamina, int32, UpdateAttack, float, UpdateAttackSpeed, float, UpdateCritical, int32, UpdateDefence);

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

	FTimerHandle RegenStaminaHandle;

	int32 PrevStamina;

	bool bCanRegenStamina;

	void RegenStamina();

public:
	void UpdateCurrentPlayerStats(const int32& AddHealth, const int32& AddStamina);

	FORCEINLINE const int32& GetCurrentPlayerHealth() const { return CurrentHealth; }

	FORCEINLINE const int32& GetCurrentPlayerStmina() const { return CurrentStamina; }

	FORCEINLINE const int32& GetCurrentAttack() const { return CurrentAttack; }

	FORCEINLINE const float& GetCurrentCritical() const { return CurrentCritcal; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = Status)
	int32 CurrentHealth;

	UPROPERTY(BlueprintReadOnly, Category = Status)
	int32 CurrentStamina;

	UPROPERTY(BlueprintReadOnly, Category = Status)
	int32 CurrentAttack;

	UPROPERTY(BlueprintReadOnly, Category = Status)
	float CurrentAttackSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Status)
	float CurrentCritcal;

	UPROPERTY(BlueprintReadOnly, Category = Status)
	int32 CurrentDefence;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Event)
	FDele_Dynamic_DefaultStatusUpdate DefaultStatusUpdateDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Event)
	FDele_Dynamic_CurrentStatusUpdate CurrentStatusUpdateDelegate;

};
