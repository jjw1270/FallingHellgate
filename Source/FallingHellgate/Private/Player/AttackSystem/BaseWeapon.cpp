// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "FHPlayerCharacter.h"

UBaseWeapon::UBaseWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	ResetNormalAttackCount();
}

void UBaseWeapon::EventNormalAttack_Implementation(ACharacter* OwnCharacter)
{
	if (NormalAttackCount > MaxNormalAttackCount)
	{
		ResetNormalAttackCount();
	}

	Attack(OwnCharacter, false);

	NormalAttackCount++;
}

void UBaseWeapon::EventSmashAttack_Implementation(ACharacter* OwnCharacter)
{
	Attack(OwnCharacter, true);

	ResetNormalAttackCount();
}

void UBaseWeapon::SetCanAttack(bool bNewCanAttack)
{
	bCanAttack = bNewCanAttack;
}

void UBaseWeapon::ResetNormalAttackCount()
{
	NormalAttackCount = 0;
}

void UBaseWeapon::Attack(ACharacter* OwnCharacter, bool bIsSmash)
{
	AFHPlayerCharacter* PlayerCharacter = Cast<AFHPlayerCharacter>(OwnCharacter);


	
	//PlayerCharacter->Req_Attack()
}

