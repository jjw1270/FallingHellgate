// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemInfo.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FConsumableItemInfo : public FTableRowBase
{

public:
    GENERATED_USTRUCT_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Price;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEffectSelf;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bHasDuration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectValue;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxEquipNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemTextInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTexture2D> ItemImage;
};

USTRUCT(Atomic, BlueprintType)
struct FEquipmentItemInfo : public FTableRowBase
{

public:
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Price;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Stamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CriticalChance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DefensivePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemTextInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EnhancedCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTexture2D> ItemImage;
};
