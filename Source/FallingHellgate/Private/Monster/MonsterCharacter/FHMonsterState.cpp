// Fill out your copyright notice in the Description page of Project Settings.


#include "FHMonsterState.h"
#include "FHMCharacter.h"
#include "Net/UnrealNetwork.h"

AFHMonsterState::AFHMonsterState()
{
	Phase = 1;
}

void AFHMonsterState::BeginPlay()
{
	Super::BeginPlay();

	if (bIsBoss)
	{
		CurHp = 1000;
		MaxHp = 1000;
	}
	else
	{
		CurHp = 5000;
		MaxHp = 5000;
	}
}

void AFHMonsterState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFHMonsterState, CurHp);
	DOREPLIFETIME(AFHMonsterState, MaxHp);
	DOREPLIFETIME(AFHMonsterState, bIsBoss);
}


void AFHMonsterState::AddDamage_Implementation(float Damage)
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
				OwnerCharacter->PhaseChangeTrigger();
				OwnerCharacter->PhaseCheck = 2;
			}
		}
		else if (CurHp <= MaxHp * 0.4) // 체력이 40% 이하일 경우
		{
			if (Phase != 3)
			{
				Phase = 3;
				OwnerCharacter->PhaseChangeTrigger();
				OwnerCharacter->PhaseCheck = 3;
			}
		}
	
	}
	else
	{
		//일반몹 
		while (AccumulatedDamage >= 1000.0f)
		{
			AccumulatedDamage -= 1000.0f;

			if (OwnerCharacter != nullptr)
				OwnerCharacter->TakeGroggy();
		}
	}

	if (CurHp < 0)
	{
		UE_LOG(LogTemp, Log, TEXT("My Hp 0 : %f"), CurHp);
		OwnerCharacter->DoRagdoll();
	}

	UE_LOG(LogTemp, Log, TEXT("Current HP after damage: %f"), CurHp);

	OnRep_CurHp();
}

void AFHMonsterState::OnRep_CurHp()
{
	if (Fuc_Dele_UpdateHp.IsBound())
		Fuc_Dele_UpdateHp.Broadcast(CurHp, MaxHp);
}

void AFHMonsterState::OnRep_MaxHp()
{
}
