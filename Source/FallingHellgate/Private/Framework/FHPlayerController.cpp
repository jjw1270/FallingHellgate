// Fill out your copyright notice in the Description page of Project Settings.


#include "FHPlayerController.h"
// Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
// Components
#include "InventoryComponent.h"
#include "QuickSlotComponent.h"
#include "EquipmentComponent.h"
// UI
#include "FHHUD.h"
#include "HUDWidget.h"
#include "BloodScreenWidget.h"

AFHPlayerController::AFHPlayerController()
{
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComp"));
	QuickSlotComp = CreateDefaultSubobject<UQuickSlotComponent>(TEXT("QuickSlotComp"));
	EquipmentComp = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComp"));
}

void AFHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(UIMappingContext, 0);
	}

	if (IsLocalController())
	{
		check(BloodScreenClass);
		BloodScreenWidget = CreateWidget<UBloodScreenWidget>(this, BloodScreenClass);
		BloodScreenWidget->AddToViewport(99);
		BloodScreenWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AFHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MouseCursorVisibilityAction, ETriggerEvent::Started, this, &AFHPlayerController::SetMouseCursorVisibility);
		EnhancedInputComponent->BindAction<AFHPlayerController, FName>(InventoryUIAction, ETriggerEvent::Started, this, &AFHPlayerController::WidgetOnOff, FName("InventoryWidget"));
		EnhancedInputComponent->BindAction<AFHPlayerController, FName>(EquipmentUIAction, ETriggerEvent::Started, this, &AFHPlayerController::WidgetOnOff, FName("EquipmentWidget"));
	}
}

void AFHPlayerController::SetMouseCursorVisibility()
{
	if (!GetMouseCursor())
	{
		SetInputMode(FInputModeGameAndUI());
		SetShowMouseCursor(true);
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
		SetShowMouseCursor(false);
	}
}

void AFHPlayerController::WidgetOnOff(FName WidgetName)
{
	if (!HUDWidget)
	{
		AFHHUD* FHHUD = GetHUD<AFHHUD>();
		check(FHHUD);

		HUDWidget = FHHUD->GetHUDWidget();
		check(HUDWidget);
	}

	HUDWidget->SwichWidgetVisibility(this, WidgetName);
}

void AFHPlayerController::CloseAllWidgets()
{
	if (!HUDWidget)
	{
		AFHHUD* FHHUD = GetHUD<AFHHUD>();
		check(FHHUD);

		HUDWidget = FHHUD->GetHUDWidget();
		check(HUDWidget);
	}

	HUDWidget->CloseAllWidgets();
}

void AFHPlayerController::OpenBackgroundWidgets()
{
	if (!HUDWidget)
	{
		AFHHUD* FHHUD = GetHUD<AFHHUD>();
		check(FHHUD);

		HUDWidget = FHHUD->GetHUDWidget();
		check(HUDWidget);
	}

	HUDWidget->OpenBackgroundWidgets();
}

void AFHPlayerController::ShowBloodScreen(bool bIsDead)
{
	if (BloodScreenWidget)
	{
		BloodScreenWidget->SetVisibility(ESlateVisibility::Visible);
		BloodScreenWidget->ShowBloodScreen(bIsDead);
	}
}
