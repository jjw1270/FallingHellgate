// Fill out your copyright notice in the Description page of Project Settings.
#include "AttackNotify.h"
#include "FHMCharacter.h"
#include "Weapon.h"
#include "MonsterWeaponInterface.h"

void UAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    AFHMCharacter* Character = Cast<AFHMCharacter>(MeshComp->GetOwner());
    if (IsValid(Character) == false)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Character in UAttackNotify::Notify"));
        return;
    }

    IMonsterWeaponInterface* InterfaceObj = Cast<IMonsterWeaponInterface>(Character->Weapon);

    // Check if Weapon is valid
    if (IsValid(Character->Weapon) == false)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Weapon in UAttackNotify::Notify"));
        return;
    }

    InterfaceObj->Execute_EventAttack(Character->Weapon);
}
