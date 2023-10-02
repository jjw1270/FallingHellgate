// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePlayerController.h"
#include "TitleWidget.h"

void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

    if (!TitleWidgetClass)
    {
        return;
    }

    TitleWidget = CreateWidget<UTitleWidget>(GetWorld(), TitleWidgetClass);
    TitleWidget->AddToViewport();

    APlayerController* player0 = GetWorld()->GetFirstPlayerController();
    if (IsValid(player0))
    {
        player0->SetInputMode(FInputModeUIOnly());
        player0->bShowMouseCursor = true;
    }
}
