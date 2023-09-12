// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InteractionInterface.h"
#include "NPCBase.generated.h"

UCLASS()
class FALLINGHELLGATE_API ANPCBase : public ACharacter, public IInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interface)
	void EventInteraction(ACharacter* OwnCharacter);

	virtual void EventInteraction_Implementation(ACharacter* OwnCharacter) override;


};
