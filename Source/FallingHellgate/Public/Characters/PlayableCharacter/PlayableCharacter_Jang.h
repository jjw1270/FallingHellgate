// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayableCharacter.h"
#include "InputActionValue.h"
#include "PlayableCharacter_Jang.generated.h"

/**
 * This is Temp class for develop _ Jang Yunje
 */
UCLASS()
class FALLINGHELLGATE_API APlayableCharacter_Jang : public APlayableCharacter
{
	GENERATED_BODY()
	
protected:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	virtual void Move(const FInputActionValue& Value) override;

	/** Called for looking input */
	virtual void Look(const FInputActionValue& Value) override;

private:
	bool bIsForwardMoveFirstPressed;
};
