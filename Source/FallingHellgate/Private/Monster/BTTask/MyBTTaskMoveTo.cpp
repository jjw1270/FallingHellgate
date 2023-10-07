// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTaskMoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIDataTable.h"
#include "AIController.h"
#include "FHMCharacter.h"
#include "Spear.h"
#include "Sword.h"
#include "Bow.h"

UMyBTTaskMoveTo::UMyBTTaskMoveTo()
{
	NodeName = TEXT("Move To Target");
}

EBTNodeResult::Type UMyBTTaskMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	UClass* WeaponType = Cast<UClass>(BlackboardComp->GetValueAsClass(TEXT("WeaponType")));

	float AcceptanceRadius;

	if (WeaponType == ASpear::StaticClass() || WeaponType == ASword::StaticClass())
	{
		AcceptanceRadius = 200.0f;
	}
	else if (WeaponType == ABow::StaticClass())
	{
		AcceptanceRadius = 1200.0f;
	}
	else
	{
		// default value or other handling.
		AcceptanceRadius = 100.0f;
	}

	FVector TargetLocation = BlackboardComp->GetValueAsVector(FName("TargetLocation"));

	if (AIController)
	{
		EPathFollowingRequestResult::Type Result = AIController->MoveToLocation(TargetLocation, AcceptanceRadius);
		return Result == EPathFollowingRequestResult::RequestSuccessful ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;;
}
