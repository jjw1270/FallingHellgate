// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AIDataTable.generated.h"


UENUM(BlueprintType)
enum class EEnemyAIState : uint8
{
    M_Patrol UMETA(DisplayName = "Patrol"),
    M_Chase UMETA(DisplayName = "Chase"),
    M_Battle UMETA(DisplayName = "Battle")
};

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UAIDataTable : public UDataTable
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterState")
    EEnemyAIState AIState;
};
