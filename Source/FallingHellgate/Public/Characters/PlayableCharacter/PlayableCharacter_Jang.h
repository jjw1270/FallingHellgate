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
	/** interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Interaction(const FInputActionValue& Value);
};
