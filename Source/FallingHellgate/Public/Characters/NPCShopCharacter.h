// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBase.h"
#include "NPCShopCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API ANPCShopCharacter : public ANPCBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = NPC)
	FText NPCMenu1Text;
	
public:
	virtual void EventInteraction_Implementation(ACharacter* OwnCharacter) override;

protected:
	UFUNCTION()
	void OpenShop();
};
