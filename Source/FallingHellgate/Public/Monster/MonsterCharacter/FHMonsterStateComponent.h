// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FHMonsterStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_Dynamic_UpdateHpTwoParams, float, CurHp, float, MaxHp);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLINGHELLGATE_API UFHMonsterStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFHMonsterStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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
	
	UPROPERTY(Replicated)
	int32 AccumulatedDamage;

public:
	UFUNCTION(Server, Reliable, Category = "MonsterState")
		virtual void AddDamage(float Damage);

public:
	UFUNCTION()
	void OnRep_CurHp();

	UFUNCTION()
	void OnRep_MaxHp();

public:
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "MonsterState")
	FDele_Dynamic_UpdateHpTwoParams Fuc_Dele_DynamicUpdateHp;


		
};
