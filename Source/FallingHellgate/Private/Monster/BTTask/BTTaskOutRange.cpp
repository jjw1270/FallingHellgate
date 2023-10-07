#include "BTTaskOutRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIDataTable.h"
#include "AIController.h"
#include "FHMCharacter.h"
#include "Spear.h"
#include "Sword.h"
#include "Bow.h"
#include "GreatSword.h"


UBTTaskOutRange::UBTTaskOutRange()
{
	NodeName = TEXT("Out of AttackRange");
}

EBTNodeResult::Type UBTTaskOutRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);


	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	AFHMCharacter* ControlledPawn = Cast<AFHMCharacter>(AIController->GetPawn());
	FVector EnemyLocation = ControlledPawn->GetActorLocation();

	if (BlackboardComp->GetValueAsObject(FName("TargetActor")) == nullptr)
		return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("TargetActor")));
	FVector PlayerLocation = TargetActor->GetActorLocation();

	UClass* WeaponType = Cast<UClass>(BlackboardComp->GetValueAsClass(TEXT("WeaponType")));


	float TargetDist = FVector::Dist(EnemyLocation, PlayerLocation);
	BlackboardComp->SetValueAsFloat(TEXT("TargetDistance"), TargetDist);

	if (WeaponType == ASpear::StaticClass() || WeaponType == ASword::StaticClass() || WeaponType == AGreatSword::StaticClass())
	{
		if (BlackboardComp->GetValueAsFloat(TEXT("TargetDistance")) >= 200)
		{
			BlackboardComp->SetValueAsEnum(FName("AIState"), static_cast<uint8>(EEnemyAIState::M_Patrol));
			return EBTNodeResult::Failed;
		}
		else
		{
			ControlledPawn->AttackTrigger();

			return EBTNodeResult::Succeeded;
		}
	}
	else if (WeaponType == ABow::StaticClass())
	{

		if (BlackboardComp->GetValueAsFloat(TEXT("TargetDistance")) >= 1300)
		{
			BlackboardComp->SetValueAsEnum(FName("AIState"), static_cast<uint8>(EEnemyAIState::M_Patrol));
			return EBTNodeResult::Failed;
		}
		else
		{
			ControlledPawn->AttackTrigger();

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
