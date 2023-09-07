// Fill out your copyright notice in the Description page of Project Settings.


#include "FHHUD.h"
#include "HUDWidget.h"

void AFHHUD::BeginPlay()
{
	Super::BeginPlay();

	// Create HUDWidget
	check(HUDWidgetClass);
	HUDWidget = CreateWidget<UHUDWidget>(GetWorld(), HUDWidgetClass);
	HUDWidget->AddToViewport();
}

UHUDWidget* AFHHUD::GetHUDWidget()
{
	if (!IsValid(HUDWidget))
	{
		return nullptr;
	}

	return HUDWidget;
}
