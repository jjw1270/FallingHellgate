
#include "BTTaskAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIDataTable.h"
#include "AIController.h"
#include "FHMCharacter.h"
#include "Spear.h"
#include "Sword.h"
#include "Bow.h"
#include "GreatSword.h"


UBTTaskAttackRange::UBTTaskAttackRange()
{
	NodeName = TEXT("AttackRange");
}

EBTNodeResult::Type UBTTaskAttackRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	UClass* WeaponType = Cast<UClass>(BlackboardComp->GetValueAsClass(TEXT("WeaponType")));
	APawn* Monster = AIController->GetPawn();

	if (BlackboardComp->GetValueAsObject(FName("TargetActor")) == nullptr)
		return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("TargetActor")));
	FVector EnemyLocation = TargetActor->GetActorLocation();

	float TargetDist = FVector::Dist(Monster->GetActorLocation(), EnemyLocation);
	
	BlackboardComp->SetValueAsFloat(TEXT("TargetDistance"), TargetDist);


	if (WeaponType == ASpear::StaticClass() || WeaponType == ASword::StaticClass() || WeaponType == AGreatSword::StaticClass())
	{
		if (BlackboardComp->GetValueAsFloat(TEXT("TargetDistance")) < 200)
		{
			BlackboardComp->SetValueAsEnum(FName("AIState"), static_cast<uint8>(EEnemyAIState::M_Battle));
			return EBTNodeResult::Failed;
		}
		else
		{
			return EBTNodeResult::Succeeded;
		}
	}
	else if (WeaponType == ABow::StaticClass())
	{

		if (BlackboardComp->GetValueAsFloat(TEXT("TargetDistance")) < 1300)
		{
			BlackboardComp->SetValueAsEnum(FName("AIState"), static_cast<uint8>(EEnemyAIState::M_Battle));
			return EBTNodeResult::Failed;
		}
		else
		{
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
