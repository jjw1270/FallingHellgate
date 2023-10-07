// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "MyBTTaskMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UMyBTTaskMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UMyBTTaskMoveTo();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
