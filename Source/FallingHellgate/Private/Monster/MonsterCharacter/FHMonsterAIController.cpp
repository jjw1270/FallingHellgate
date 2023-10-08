#include "FHMonsterAIController.h"
#include "NavigationSystem.h"
#include "FHMCharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIDataTable.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Net/UnrealNetwork.h"
#include "FHMonsterState.h"
#include "FHPlayerCharacter.h"
#include "FHGameInstance.h"

const FName AFHMonsterAIController::TargetActor(TEXT("TargetActor"));
const FName AFHMonsterAIController::StartLocation(TEXT("StartLocation"));
const FName AFHMonsterAIController::TargetPointLocation(TEXT("TargetPointLocation"));
const FName AFHMonsterAIController::TargetDistance(TEXT("TargetDistance"));

AFHMonsterAIController::AFHMonsterAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    //AIPerception
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

    //Sight
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

    //Hearing
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));

    //AIPerception Setting
    AIPerception->ConfigureSense(*SightConfig);
    AIPerception->ConfigureSense(*HearingConfig);
    AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

    //Sight Setting
    SightConfig->SightRadius = 1400.f;
    SightConfig->LoseSightRadius = 1450.0f;
    SightConfig->PeripheralVisionAngleDegrees = 90.f;

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    SightConfig->SetMaxAge(0.1f);

    //Hearing Setting
    HearingConfig->HearingRange = 2000.0f;

    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = false;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = false;

}

void AFHMonsterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (IsValid(InPawn))
    {
        AFHMonsterState* MonsterState = NewObject<AFHMonsterState>();
        if (IsValid(MonsterState))
        {
            InPawn->SetPlayerState(MonsterState);
        }
    }
}

void AFHMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

    SetReplicates(true);

    AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AFHMonsterAIController::UpdateTarget);

    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);

        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        AFHMCharacter* Player = Cast<AFHMCharacter>(AAIController::GetPawn());

        GetBlackboardComponent()->SetValueAsVector(StartLocation, GetPawn()->GetActorLocation());
        GetBlackboardComponent()->SetValueAsEnum(FName("AIState"), static_cast<uint8>(EEnemyAIState::M_Patrol));

        //Patrol Point 抗寇贸府
        if (Player->TargetPatrol != nullptr)
        {
            FVector TargetPoint = Player->TargetPatrol->GetActorLocation();
            GetBlackboardComponent()->SetValueAsVector(TargetPointLocation, TargetPoint);
        }
        else
        {
            GetBlackboardComponent()->SetValueAsVector(TargetPointLocation, GetPawn()->GetActorLocation());
        }

    }
}

void AFHMonsterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AFHMonsterAIController::S2CDelayServerTravel_Implementation()
{
    GetWorld()->GetTimerManager().SetTimer(th_SeverTravel, this, &AFHMonsterAIController::S2CServerTravel, 5.0f, false);
}

void AFHMonsterAIController::S2CServerTravel_Implementation()
{
    UFHGameInstance* GameInst = Cast<UFHGameInstance>(GetWorld()->GetGameInstance());

    if (nullptr != GameInst)
    {
        UE_LOG(LogTemp, Warning, TEXT("GameInst is done"));
        GameInst->PerformServerTravel(TEXT("/Game/Maps/villageMain"));
        UnPossess();
    }
}

void AFHMonsterAIController::UpdateLocation_Implementation(AActor* Actor)
{
    GetBlackboardComponent()->SetValueAsEnum(FName("AIState"), static_cast<uint8>(EEnemyAIState::M_Chase));

    //if tag 贸府
    if (Cast<AFHPlayerCharacter>(Actor))
    {
        GetBlackboardComponent()->SetValueAsObject(TargetActor, Actor);
    }
}

void AFHMonsterAIController::UpdateTarget_Implementation(AActor* Actor, FAIStimulus Stimulus)
{
    if (Stimulus.WasSuccessfullySensed())
    {
        UpdateLocation(Actor);
    }
    else
    {

        GetWorldTimerManager().SetTimer(th_SightOut,[this, Actor]() { ILostSightOf(Actor); },10.f,false);
    }
}

void AFHMonsterAIController::ILostSightOf_Implementation(AActor* Actor)
{
    FVector LastSeenAt = Actor->GetActorLocation();

    GetBlackboardComponent()->SetValueAsEnum(FName("AIState"), static_cast<uint8>(EEnemyAIState::M_Patrol));
    GetBlackboardComponent()->ClearValue(TargetActor);
}

void AFHMonsterAIController::ResetSight_Implementation()
{
    GetBlackboardComponent()->SetValueAsEnum(FName("AIState"), static_cast<uint8>(EEnemyAIState::M_Patrol));
}

