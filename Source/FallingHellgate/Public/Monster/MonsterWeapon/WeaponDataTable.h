// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponDataTable.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    FWeaponData()
        : Damage(0),
          SocketToAttach(TEXT("bone1")),
          Mesh(nullptr)
    {
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    int32 Damage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SocketName")
    FName SocketToAttach;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<USkeletalMesh> Mesh;
};
/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UWeaponDataTable : public UDataTable
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterWeapon")
    FWeaponData Weapon;

};
