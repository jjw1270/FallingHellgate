// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UBTTaskAttackRange : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskAttackRange();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
