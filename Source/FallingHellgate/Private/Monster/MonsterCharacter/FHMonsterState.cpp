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

}

void AFHMonsterState::OnRep_CurHp()
{

}

void AFHMonsterState::OnRep_MaxHp()
{
}
