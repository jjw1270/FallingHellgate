// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusWidget.h"
#include "FHPlayerCharacter.h"
#include "PlayerStatusComponent.h"
#include "Components/TextBlock.h"

void UStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindStatusCompEvents();
}

void UStatusWidget::BindStatusCompEvents()
{
	AFHPlayerCharacter* PlayerCharacter = GetOwningPlayerPawn<AFHPlayerCharacter>();

	if (IsValid(PlayerCharacter))
	{
		UPlayerStatusComponent* PlayerStatusComp = PlayerCharacter->GetComponentByClass<UPlayerStatusComponent>();
		if (IsValid(PlayerStatusComp))
		{
			PlayerStatusComp->DefaultStatusUpdateDelegate.AddDynamic(this, &UStatusWidget::OnStatusUpdate);
			return;
		}
	}

	GetWorld()->GetTimerManager().SetTimer(InitTimerHandle, this, &UStatusWidget::BindStatusCompEvents, 0.1f, false);
}

void UStatusWidget::OnStatusUpdate(int32 UpdateDefaultHealth, int32 UpdateDefaultStamina, int32 UpdateDefaultAttack, float UpdateDefaultAttackSpeed, float UpdateDefaultCritical, int32 UpdateDefaultDefence)
{
	Text_Health->SetText(FText::AsNumber(UpdateDefaultHealth));
	Text_Stamina->SetText(FText::AsNumber(UpdateDefaultStamina));
	Text_Attack->SetText(FText::AsNumber(UpdateDefaultAttack));
	FNumberFormattingOptions Text_AttackSpeedTextOption;
	Text_AttackSpeedTextOption.SetMinimumFractionalDigits(1);
	Text_AttackSpeed->SetText(FText::AsNumber(UpdateDefaultAttackSpeed, &Text_AttackSpeedTextOption));
	Text_Critical->SetText(FText::AsPercent(UpdateDefaultCritical));
	Text_Defence->SetText(FText::AsNumber(UpdateDefaultDefence));
}
