// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacter_Jang.h"
#include "Components/CapsuleComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InteractionInterface.h"

void APlayableCharacter_Jang::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayableCharacter_Jang::Interaction);
	}
}

void APlayableCharacter_Jang::Interaction(const FInputActionValue& Value)
{
	TArray<AActor*> OverlappingActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors);

	double NearestLength = 99999999.0f;
	IInteractionInterface* NearestInteractiveInterfaceObj = nullptr;

	// Find NearestInteractiveInterfaceObj
	for (AActor* Target : OverlappingActors)
	{
		IInteractionInterface* TargetInterfaceObj = Cast<IInteractionInterface>(Target);
		if (!TargetInterfaceObj)
		{
			continue;
		}

		double distance = FVector::Dist(Target->GetActorLocation(), GetActorLocation());
		if (NearestLength < distance)
		{
			continue;
		}

		NearestLength = distance;

		NearestInteractiveInterfaceObj = TargetInterfaceObj;
		InteractingActor = Target;
	}

	if (!NearestInteractiveInterfaceObj)
	{
		return;
	}

	NearestInteractiveInterfaceObj->Execute_EventInteraction(InteractingActor);
}
