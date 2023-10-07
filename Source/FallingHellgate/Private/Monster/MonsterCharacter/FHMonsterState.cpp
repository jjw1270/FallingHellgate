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
		CurHp = 50000;
		MaxHp = 50000;
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


void AFHMonsterState::AddDamage(float Damage)
{
	CurHp = CurHp - Damage;

	//�׷α� ��ġ
	AccumulatedDamage += Damage;

	if (bIsBoss)
	{
		//������
		while (AccumulatedDamage >= 10000.0f)
		{
			AccumulatedDamage -= 10000.0f;

			if (OwnerCharacter != nullptr)
				OwnerCharacter->TakeGroggy();
		}

		if (CurHp <= MaxHp * 0.7 && CurHp > MaxHp * 0.4) // ü���� 70% ������ ���
		{
			if (Phase != 2)
			{
				Phase = 2;
				OwnerCharacter->PhaseChangeTrigger();
				OwnerCharacter->PhaseCheck = 2;
			}
		}
		else if (CurHp <= MaxHp * 0.4) // ü���� 40% ������ ���
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
		//�Ϲݸ� 
		while (AccumulatedDamage >= 1000.0f)
		{
			AccumulatedDamage -= 1000.0f;

			if (OwnerCharacter != nullptr)
				OwnerCharacter->TakeGroggy();
		}
	}

	if (CurHp < 0)
	{
		OwnerCharacter->DoRagdoll();
	}

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
