// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPartyWidget.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"

void UFindPartyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Refresh->OnClicked.AddDynamic(this, &UFindPartyWidget::OnBtn_RefreshClicked);
}

void UFindPartyWidget::OnBtn_RefreshClicked()
{
	FindParty();
}

void UFindPartyWidget::FindParty_Implementation()
{
	BackgroundBlur_FindParty->SetVisibility(ESlateVisibility::Visible);
}

void UFindPartyWidget::FindPartyComplete_Implementation(const TArray<FBlueprintSessionResult>& SessionResult)
{
	BackgroundBlur_FindParty->SetVisibility(ESlateVisibility::Collapsed);
}
