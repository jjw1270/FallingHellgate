// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCBase.h"
#include "FallingHellgate.h"
#include "FHPlayerCharacter.h"
#include "FHPlayerController.h"
#include "NPCWidget.h"
#include "Components/Button.h"

// Sets default values
ANPCBase::ANPCBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANPCBase::BeginPlay()
{
	Super::BeginPlay();
	
    CHECK_VALID(NPCWidgetClass);
}

void ANPCBase::UpdateCharacterRotation()
{
    if (!TargetPlayer)
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        return;
    }

    // Calculate the vector pointing from the character to the target.
    FVector TargetDirection = TargetPlayer->GetActorLocation() - GetActorLocation();

    // Calculate the rotation needed to face the target.
    FRotator TargetRotation = TargetDirection.Rotation();

    // Smoothly interpolate the character's rotation to face the target.
    FRotator InterpolatedRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), InterpolationSpeed);

    // Apply the new rotation to the character.
    SetActorRotation(InterpolatedRotation);

    if (FMath::IsNearlyEqual(InterpolatedRotation.Yaw, TargetRotation.Yaw, 5.0f))
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
    }
}

void ANPCBase::EventInteraction_Implementation(ACharacter* OwnCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact NPC"));

    TargetPlayer = Cast<AFHPlayerCharacter>(OwnCharacter);
	CHECK_VALID(TargetPlayer);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANPCBase::UpdateCharacterRotation, UpdateInterval, true);

    AFHPlayerController* PC = TargetPlayer->GetController<AFHPlayerController>();
    CHECK_VALID(PC);

    PC->CloseAllWidgets();

    if (!PC->IsLocalPlayerController())
    {
        return;
    }
    NPCWidget = CreateWidget<UNPCWidget>(PC, NPCWidgetClass);
    NPCWidget->Btn_Exit->OnClicked.AddDynamic(this, &ANPCBase::EndInteraction);

    NPCWidget->AddToViewport(99);

    PC->SetInputMode(FInputModeUIOnly());
    PC->SetShowMouseCursor(true);
}

void ANPCBase::EndInteraction()
{
    TargetPlayer->InteractingActor = nullptr;

    AFHPlayerController* PC = TargetPlayer->GetController<AFHPlayerController>();
    CHECK_VALID(PC);

    if (!NPCWidget)
    {
        return;
    }

    NPCWidget->RemoveFromParent();

    PC->SetInputMode(FInputModeGameOnly());
    PC->SetShowMouseCursor(false);
}
