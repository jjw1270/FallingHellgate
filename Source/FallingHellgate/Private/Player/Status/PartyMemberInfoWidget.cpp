// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyMemberInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "PlayerStatusComponent.h"

void UPartyMemberInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();


}

void UPartyMemberInfoWidget::InitPartyMemberInfoWidget(const FText& NewMemberName, class UPlayerStatusComponent* PlayerStatusComp)
{
	TextBlock_MemberName->SetText(NewMemberName);

	PlayerStatusComp->DefaultStatusUpdateDelegate.AddDynamic(this, &UPartyMemberInfoWidget::OnDefaultStatusUpdated);
	PlayerStatusComp->CurrentStatusUpdateDelegate.AddDynamic(this, &UPartyMemberInfoWidget::OnCurrentStatusUpdated);

	PlayerStatusComp->UpdateDefaultStatusUI();
	PlayerStatusComp->UpdateCurrentStatusUI();
}

FText UPartyMemberInfoWidget::GetPartyMemberName()
{
	return TextBlock_MemberName->GetText();
}

void UPartyMemberInfoWidget::OnDefaultStatusUpdated(int32 UpdateHealth, int32 UpdateStamina, int32 UpdateAttack, float UpdateAttackSpeed, float UpdateCritical, int32 UpdateDefence)
{
	DefaultHP = UpdateHealth;

	ProgressBar_HP->SetPercent((float)CurHP / DefaultHP);
}

void UPartyMemberInfoWidget::OnCurrentStatusUpdated(int32 UpdateHealth, int32 UpdateStamina, int32 UpdateAttack, float UpdateAttackSpeed, float UpdateCritical, int32 UpdateDefence)
{
	CurHP = UpdateHealth;

	ProgressBar_HP->SetPercent((float)CurHP / DefaultHP);
}
