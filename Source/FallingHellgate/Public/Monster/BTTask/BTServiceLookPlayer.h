// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTServiceLookPlayer.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UBTServiceLookPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTServiceLookPlayer();
	
	UFUNCTION(Category = "BTTask")
	void CheckTarget(AAIController* AIController, AActor* TargetActor,	UBlackboardComponent* BlackboardComp, float DeltaSeconds);

	UFUNCTION(Category = "BTTask")
	void RotateTowardsTarget(APawn* Enermy, AActor* TargetActor, float DeltaSeconds);

	float TimeSinceLastSight = 0.f;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
