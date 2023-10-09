// Fill out your copyright notice in the Description page of Project Settings.


#include "FHMonsterStateComponent.h"
#include "FHMonsterAIController.h"
#include "FHMCharacter.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UFHMonsterStateComponent::UFHMonsterStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Phase = 1;
}


// Called when the game starts
void UFHMonsterStateComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bIsBoss)
	{
		CurHp = 3000;
		MaxHp = 3000;
	}
	else
	{
		CurHp = 1000;
		MaxHp = 1000;
	}
}

void UFHMonsterStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFHMonsterStateComponent, CurHp);
	DOREPLIFETIME(UFHMonsterStateComponent, MaxHp);
	DOREPLIFETIME(UFHMonsterStateComponent, bIsBoss);
	DOREPLIFETIME(UFHMonsterStateComponent, AccumulatedDamage);
	DOREPLIFETIME(UFHMonsterStateComponent, Phase);
}


// Called every frame
void UFHMonsterStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFHMonsterStateComponent::AddDamage_Implementation(float Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Enermy Take %f"), Damage);

	CurHp = CurHp - Damage;

	//그로기 수치
	AccumulatedDamage += Damage;

	if (bIsBoss)
	{
		//보스몹
		while (AccumulatedDamage >= 100.0f)
		{
			AccumulatedDamage -= 100.0f;

			if (OwnerCharacter != nullptr)
				OwnerCharacter->TakeGroggy();
		}

		if (CurHp <= MaxHp * 0.7 && CurHp > MaxHp * 0.4) // 체력이 70% 이하일 경우
		{
			if (Phase != 2)
			{
				Phase = 2;
				OwnerCharacter->PhaseCheck = 2;
				OwnerCharacter->PhaseChangeTrigger();
			}
		}
		else if (CurHp <= MaxHp * 0.4) // 체력이 40% 이하일 경우
		{
			if (Phase != 3)
			{
				Phase = 3;
				OwnerCharacter->PhaseCheck = 3;
				OwnerCharacter->PhaseChangeTrigger();
			}
		}

	}
	else
	{
		//일반몹 
		while (AccumulatedDamage >= 200.0f)
		{
			AccumulatedDamage -= 200.0f;

			if (OwnerCharacter != nullptr)
				OwnerCharacter->S2CTakeGroggy();
		}
	}

	if (CurHp < 0)
	{
		UE_LOG(LogTemp, Log, TEXT("My Hp 0 : %f"), CurHp);
		OwnerCharacter->DoRagdoll();

		AFHMonsterAIController* AIController = Cast<AFHMonsterAIController>(OwnerCharacter->GetController());

		if (bIsBoss)
		{
			OwnerCharacter->S2CStageClear();

			if (AIController != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("AIController is done"));
				AIController->S2CDelayServerTravel();
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Current HP after damage: %f"), CurHp);

	OnRep_CurHp();
}


void UFHMonsterStateComponent::OnRep_CurHp()
{
	if (Fuc_Dele_DynamicUpdateHp.IsBound())
		Fuc_Dele_DynamicUpdateHp.Broadcast(CurHp, MaxHp);
}

void UFHMonsterStateComponent::OnRep_MaxHp()
{
}


