// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatWidget.h"
#include "FallingHellgate.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "FHHUD.h"
#include "HUDWidget.h"
#include "FHPlayerCharacter.h"
#include "PlayerStatusComponent.h"

void UPlayerStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind UI Drag Event
	UIDragBtn->OnPressed.AddDynamic(this, &UPlayerStatWidget::OnDragBtnPressed);
	UIDragBtn->OnReleased.AddDynamic(this, &UPlayerStatWidget::OnDragBtnReleased);

	// Bind QuickSlotComponent Delegates
	BindStatusCompEvents();
}

void UPlayerStatWidget::BindStatusCompEvents()
{
	AFHPlayerCharacter* PlayerChar = GetOwningPlayerPawn<AFHPlayerCharacter>();

	if (IsValid(PlayerChar))
	{
		PlayerStatusComp = PlayerChar->GetPlayerStatusComp();
		if (IsValid(PlayerStatusComp))
		{
			PlayerStatusComp->DefaultStatusUpdateDelegate.AddDynamic(this, &UPlayerStatWidget::OnDefaultStatusUpdated);
			PlayerStatusComp->CurrentStatusUpdateDelegate.AddDynamic(this, &UPlayerStatWidget::OnCurrentStatusUpdated);

			return;
		}
	}

	GetWorld()->GetTimerManager().SetTimer(InitTimerHandle, this, &UPlayerStatWidget::BindStatusCompEvents, 0.1f, false);
}

void UPlayerStatWidget::OnDefaultStatusUpdated(int32 UpdateHealth, int32 UpdateStamina, int32 UpdateAttack, float UpdateAttackSpeed, float UpdateCritical, int32 UpdateDefence)
{
	DefaultHP = UpdateHealth;
	UpdateHPProgressBar();

	DefaultStamina = UpdateStamina;
	UpdateStaminaProgressBar();

	TextBlock_MaxHP->SetText(FText::AsNumber(DefaultHP));
	TextBlock_MaxStamina->SetText(FText::AsNumber(DefaultStamina));
}

void UPlayerStatWidget::OnCurrentStatusUpdated(int32 UpdateHealth, int32 UpdateStamina, int32 UpdateAttack, float UpdateAttackSpeed, float UpdateCritical, int32 UpdateDefence)
{
	CurHP = UpdateHealth;
	UpdateHPProgressBar();

	CurStamina = UpdateStamina;
	UpdateStaminaProgressBar();

	TextBlock_CurHP->SetText(FText::AsNumber(CurHP));
	TextBlock_CurStamina->SetText(FText::AsNumber(CurStamina));
}

void UPlayerStatWidget::UpdateHPProgressBar()
{
	ProgressBar_HP->SetPercent((float)CurHP / DefaultHP);
}

void UPlayerStatWidget::UpdateStaminaProgressBar()
{
	ProgressBar_Stamina->SetPercent((float)CurStamina / DefaultStamina);
}

void UPlayerStatWidget::OnDragBtnPressed()
{
	if (!HUDWidget)
	{
		AFHHUD* HUD = Cast<AFHHUD>(GetOwningPlayer()->GetHUD());
		CHECK_VALID(HUD);

		HUDWidget = HUD->GetHUDWidget();
	}

	HUDWidget->WidgetDragStart(this);
}

void UPlayerStatWidget::OnDragBtnReleased()
{
	CHECK_VALID(HUDWidget);

	HUDWidget->WidgetDragEnd();
}
