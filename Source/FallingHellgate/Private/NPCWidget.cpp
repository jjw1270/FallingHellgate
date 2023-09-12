// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Spacer.h"

void UNPCWidget::SetNPCNameText(FText NPCName)
{
	Text_NPCName->SetText(NPCName);
}

void UNPCWidget::SetNPCDialoguesText(FText NPCDialogues)
{
	Text_NPCDialogues->SetText(NPCDialogues);
}

void UNPCWidget::HideUnBoundMenu()
{
	if (!Btn_NPCMenu2->OnClicked.IsBound())
	{
		Btn_NPCMenu2->SetVisibility(ESlateVisibility::Collapsed);
		Spacer_Menu2->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (!Btn_NPCMenu1->OnClicked.IsBound())
	{
		Btn_NPCMenu1->SetVisibility(ESlateVisibility::Collapsed);
		Spacer_Menu1->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UNPCWidget::SetMenu(int32 MenuIndex, FText MenuText)
{
	switch (MenuIndex)
	{
	case 1:
		Text_NPCMenu1->SetText(MenuText);
		break;
	case 2:
		Text_NPCMenu2->SetText(MenuText);
		break;
	default:
		break;
	}
}
