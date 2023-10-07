// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FHMonsterState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_UpdateHp_TwoParams, float, CurHp, float, MaxHp);
/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API AFHMonsterState : public APlayerState
{
	GENERATED_BODY()

public:
	AFHMonsterState();

public:
	virtual void BeginPlay() override;

public:
	class AFHMCharacter* OwnerCharacter = nullptr;

public:
	UPROPERTY(blueprintReadOnly, ReplicatedUsing = OnRep_CurHp, Category = "MonsterState")
	float CurHp;

	UPROPERTY(blueprintReadOnly, ReplicatedUsing = OnRep_MaxHp, Category = "MonsterState")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "MonsterState")
	bool bIsBoss;

	UPROPERTY(Replicated)
	int32 Phase;

public:
	UFUNCTION(BlueprintCallable, Category = "MonsterState")
	virtual void AddDamage(float Damage);

public:
	UFUNCTION()
	void OnRep_CurHp();

	UFUNCTION()
	void OnRep_MaxHp();

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "MonsterState")
	FDele_UpdateHp_TwoParams Fuc_Dele_UpdateHp;

	float AccumulatedDamage = 0.0f;
};
