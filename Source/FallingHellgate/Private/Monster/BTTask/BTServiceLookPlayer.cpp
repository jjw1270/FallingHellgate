// Fill out your copyright notice in the Description page of Project Settings.


#include "BTServiceLookPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "AIDataTable.h"
#include "Kismet/KismetMathLibrary.h"

UBTServiceLookPlayer::UBTServiceLookPlayer()
{
	bNotifyBecomeRelevant = true;

	Interval = 0.01f;

	RandomDeviation = 0.0f;

	NodeName = TEXT("Look Target");
}

void UBTServiceLookPlayer::RotateTowardsTarget(APawn* Enermy, AActor* TargetActor, float DeltaSeconds)
{
	FRotator LookLocation = UKismetMathLibrary::FindLookAtRotation(Enermy->GetActorLocation(), TargetActor->GetActorLocation());
	LookLocation.Pitch = 0;
	FRotator InterToRotation = FMath::RInterpTo(Enermy->GetActorRotation(), LookLocation, DeltaSeconds, 5.0f);
	Enermy->SetActorRotation(InterToRotation);
}

void UBTServiceLookPlayer::CheckTarget(AAIController* AIController, AActor* TargetActor,UBlackboardComponent* BlackboardComp, float DeltaSeconds)
{
	float Distance = BlackboardComp->GetValueAsFloat(TEXT("TargetDistance"));

	if (IsValid(TargetActor) && Distance >= 1400.f)
	{
		TimeSinceLastSight = 0.f;
		
	}
	else
	{
		TimeSinceLastSight += DeltaSeconds;

		if (TimeSinceLastSight >= 15.f)
		{
			BlackboardComp->ClearValue(FName("TargetActor"));
			BlackboardComp->SetValueAsEnum(FName("AIState"), static_cast<uint8>(EEnemyAIState::M_Patrol));
			TimeSinceLastSight = 0.f;
		}	
	}
}

void UBTServiceLookPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	APawn* Enermy = AIController->GetPawn();

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("TargetActor")));

	if (IsValid(TargetActor))
	{
		RotateTowardsTarget(Enermy, TargetActor, DeltaSeconds);
		CheckTarget(AIController, TargetActor, BlackboardComp, DeltaSeconds);
	}
}