// Fill out your copyright notice in the Description page of Project Settings.


#include "FHPlayerController.h"
#include "FallingHellgate.h"
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
#include "EnterDungeonWidget.h"
#include "PartyInfoWidget.h"

#include "FHPlayerCharacter.h"
#include "PlayerStatusComponent.h"
#include "EngineUtils.h"

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

	APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}

	if (IsLocalController())
	{
		check(BloodScreenClass);
		BloodScreenWidget = CreateWidget<UBloodScreenWidget>(this, BloodScreenClass);
		BloodScreenWidget->AddToViewport(99);
		BloodScreenWidget->SetVisibility(ESlateVisibility::Collapsed);


	}

	FTimerHandle SyncPlayerStatsHandle;
	GetWorldTimerManager().SetTimer(SyncPlayerStatsHandle, [&]() { C2S_SyncPlayerStats(); }, 3.f, false);
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

void AFHPlayerController::S2C_CreateEnterDungeonWidget_Implementation(const FString& NewDungeonName)
{
	FString DungeonName = NewDungeonName;

	UEnterDungeonWidget* EnterDungeonWidget = CreateWidget<UEnterDungeonWidget>(this, EnterDungeonWidgetClass);
	EnterDungeonWidget->SetDungeonName(DungeonName);
	EnterDungeonWidget->AddToViewport();

	SetInputMode(FInputModeGameAndUI());
	SetShowMouseCursor(true);
}

void AFHPlayerController::C2S_SyncPlayerStats_Implementation()
{
	if (GetWorld()->GetMapName().Contains(TEXT("Village")))
	{
		return;
	}

	//Find All PC
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		AFHPlayerController* PC = Cast<AFHPlayerController>(*Iter);
		if (PC)
		{
			PC->S2C_SyncPlayerStats();
		}
	}
}

void AFHPlayerController::S2C_SyncPlayerStats_Implementation()
{
	AFHHUD* FHHUD = GetHUD<AFHHUD>();
	CHECK_VALID(FHHUD);
	UPartyInfoWidget* PartyInfoWidget = FHHUD->GetHUDWidget()->GetPartyInfoWidget();
	CHECK_VALID(PartyInfoWidget);

	for (TActorIterator<AFHPlayerCharacter> Iter(GetWorld()); Iter; ++Iter)
	{
		AFHPlayerCharacter* PlayerChar = *Iter;
		if (PlayerChar)
		{
			UPlayerStatusComponent* PlayerStatusComp = PlayerChar->GetPlayerStatusComp();
			if (PlayerStatusComp)
			{
				PartyInfoWidget->SetPartyMember(PlayerStatusComp->GetPlayerName(), PlayerStatusComp, true);
			}
		}
	}
}
