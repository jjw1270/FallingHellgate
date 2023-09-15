// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// NEED TO CHANGE IT TO INCLUDING INTERACTION //

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
	};

/**
 *
 */
class FALLINGHELLGATE_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Interaction)
	void EventInteraction(ACharacter* OwnCharacter);
};
