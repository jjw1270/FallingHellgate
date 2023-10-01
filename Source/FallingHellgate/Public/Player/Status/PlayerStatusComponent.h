// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FHGameInstance.h"
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
	void UpdateDefaultStatusToGameInst();

	void UpdateDefaultStatusUI();

	void UpdateCurrentStatusUI();

protected:
	UFUNCTION()
	void OnWeaponUpdate(class UItemData* UpdateEquipItem, const bool& bIsEquip);

	UFUNCTION()
	void OnArmorUpdate(const EArmorType& UpdateArmorType, class UItemData* UpdateEquipItem, const bool& bIsEquip);

protected:
	FTimerHandle RegenStaminaHandle;

	int32 PrevStamina;

	bool bCanRegenStamina;

	void RegenStamina();

	FTimerHandle RegenHealthHandle;

	int32 PrevHealth;

	bool bCanRegenHealth;

	void RegenHealth();

	UPROPERTY(VisibleAnywhere)
	FPlayerStats DefaultPlayerStats;

	UPROPERTY(VisibleAnywhere)
	FPlayerStats CurrentPlayerStats;

protected:
	UFUNCTION(Server, Reliable)
	void C2S_UpdateDefaultPlayerStats(const FPlayerStats& NewDefultPlayerStats);

	UFUNCTION(NetMulticast, Reliable)
	void S2M_UpdateDefaultPlayerStats(const FPlayerStats& NewDefultPlayerStats);

	UFUNCTION(Server, Reliable)
	void C2S_UpdateCurrentPlayerStats(const FPlayerStats& NewCurrentPlayerStats);

	UFUNCTION(NetMulticast, Reliable)
	void S2M_UpdateCurrentPlayerStats(const FPlayerStats& NewCurrentPlayerStats);

public:
	void InitCurrentPlayerStats();

	UFUNCTION()
	void UpdateDefaultPlayerStats(const bool& bIsEquip, const int32& AddHealth, const int32& AddStamina, const int32& AddAttack, const float& AddAttackSpeed, const float& AddCritcal, const int32& AddDefence);

	void UpdateCurrentPlayerStats(const int32& AddHealth, const int32& AddStamina, const int32& AddAttack = 0, const float& AddAttackSpeed = 0, const float& AddCritcal = 0, const int32& AddDefence = 0);

	FORCEINLINE const FPlayerStats& GetDefaultPlayerStats() const { return DefaultPlayerStats; }

	FORCEINLINE const FPlayerStats& GetCurrentPlayerStats() const { return CurrentPlayerStats; }

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Event)
	FDele_Dynamic_DefaultStatusUpdate DefaultStatusUpdateDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = Event)
	FDele_Dynamic_CurrentStatusUpdate CurrentStatusUpdateDelegate;

};
