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
	ANPCBase();

protected:
	UPROPERTY()
	class AFHPlayerCharacter* TargetPlayer;

	// Interpolation speed for smoothly turning towards the target.
	float InterpolationSpeed = 5.0f;

	// Time interval between updates.
	float UpdateInterval = 0.01f;

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UNPCWidget> NPCWidgetClass;

	UPROPERTY()
	class UNPCWidget* NPCWidget;

protected:
	UPROPERTY(EditAnywhere, Category = NPC)
	FText NPCName;

	UPROPERTY(EditAnywhere, Category = NPC)
	FText NPCDialogues;

protected:
	virtual void BeginPlay() override;

	void UpdateCharacterRotation();

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interface)
	void EventInteraction(ACharacter* OwnCharacter);

	virtual void EventInteraction_Implementation(ACharacter* OwnCharacter) override;

	virtual void EndInteraction();
};
