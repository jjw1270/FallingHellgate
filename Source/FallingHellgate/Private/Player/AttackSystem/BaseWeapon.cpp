// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "FallingHellgate.h"
#include "FHPlayerCharacter.h"

UBaseWeapon::UBaseWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void UBaseWeapon::SetEquipMesh(USkeletalMesh* NewArmorMesh, const bool& bIsEquip, class UAnimMontage* NewNormalAttackMontage, class UAnimMontage* NewSmashAttackMontage)
{
	// if UnEquip, Set default Mesh
	if (!bIsEquip)
	{
		SetSkeletalMeshAsset(DefaultSkeletalMeshAsset);
		NormalAttackMontage = nullptr;
		SmashAttackMontage = nullptr;

		return;
	}

	CHECK_VALID(NewArmorMesh);
	SetSkeletalMeshAsset(NewArmorMesh);

	NormalAttackMontage = NewNormalAttackMontage;
	SmashAttackMontage = NewSmashAttackMontage;

	ResetNormalAttackCount();
}

void UBaseWeapon::EventNormalAttack_Implementation(ACharacter* OwnCharacter, FRotator AttackRot)
{
	GetWorld()->GetTimerManager().ClearTimer(ResetAttackCountHandle);

	if (NormalAttackCount == MaxNormalAttackCount)
	{
		ResetNormalAttackCount();
	}

	NormalAttackCount++;

	Attack(OwnCharacter, false, AttackRot);
}

void UBaseWeapon::EventSmashAttack_Implementation(ACharacter* OwnCharacter, FRotator AttackRot)
{
	GetWorld()->GetTimerManager().ClearTimer(ResetAttackCountHandle);

	Attack(OwnCharacter, true, AttackRot);

	ResetNormalAttackCount();
}

void UBaseWeapon::ResetNormalAttackCount()
{
	NormalAttackCount = 0;
}

void UBaseWeapon::Attack(ACharacter* OwnCharacter, bool bIsSmash, FRotator AttackRot)
{
	AFHPlayerCharacter* PlayerCharacter = Cast<AFHPlayerCharacter>(OwnCharacter);
	
	if (!bIsSmash)
	{
		FString SectionName = FString::Printf(TEXT("NAt%d"), NormalAttackCount);

		CHECK_VALID(NormalAttackMontage);
		PlayerCharacter->Req_Attack(AttackRot, NormalAttackMontage, FName(*SectionName));
	}
	else
	{
		FString SectionName = FString::Printf(TEXT("SAt%d"), NormalAttackCount);

		CHECK_VALID(SmashAttackMontage);
		PlayerCharacter->Req_Attack(AttackRot, SmashAttackMontage, FName(*SectionName));
	}

	GetWorld()->GetTimerManager().SetTimer(ResetAttackCountHandle, this, &UBaseWeapon::ResetNormalAttackCount, 1.3f, false);
}
