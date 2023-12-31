// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FHMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API AFHMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AFHMonsterAIController();

	virtual void OnPossess(APawn* InPawn) override;

protected:


	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

public:
	static const FName StartLocation;
	static const FName TargetPointLocation;
	static const FName TargetDistance;
	static const FName TargetActor;
	
	UFUNCTION(Server, Reliable)
	void UpdateLocation(AActor* Actor);

	//Search to sight for Target

	UPROPERTY(VisibleAnywhere, Category = "AIPerception")
	class UAIPerceptionComponent* AIPerception;

	UPROPERTY(VisibleAnywhere, Category = "AIPerception")
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, Category = "AIPerception")
	class UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	FVector LastKnownLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	AActor* CurrentTarget;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	FVector TargetPatrolLocation;

	UFUNCTION(Server, Reliable, Category = "Monster")
	void UpdateTarget(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION(Server, Reliable, Category = "Monster")
	void ILostSightOf(AActor* Actor);

	UFUNCTION(Server, Reliable, Category = "Monster")
	void ResetSight();

	UFUNCTION(Server, Reliable, Category = "Monster")
	void S2CServerTravel();

	UFUNCTION(NetMulticast, Reliable, Category = "Monster")
	
	void S2CDelayServerTravel();

public:

	FTimerHandle th_SightOut;

	FTimerHandle th_SeverTravel;

private:
	UPROPERTY(EditAnywhere, Category = "Monster")
	class UBehaviorTree* AIBehavior;
};
