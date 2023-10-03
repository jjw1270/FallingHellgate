// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatePartyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "NPCFindParty.h"

void UCreatePartyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_SubNum->OnClicked.AddDynamic(this, &UCreatePartyWidget::OnBtn_SubNumClicked);
	Btn_AddNum->OnClicked.AddDynamic(this, &UCreatePartyWidget::OnBtn_AddNumClicked);
	Btn_CreateParty->OnClicked.AddDynamic(this, &UCreatePartyWidget::OnBtn_CreatePartyClicked);
	Btn_CloseCreateParty->OnClicked.AddDynamic(this, &UCreatePartyWidget::OnBtn_CloseCreatePartyClicked);

	MaxNum = 4;
}

void UCreatePartyWidget::OnBtn_SubNumClicked()
{
	MaxNum--;

	if (MaxNum < 1)
	{
		MaxNum = 1;
	}

	TextBlock_PartyMaxNum->SetText(FText::AsNumber(MaxNum));
}

void UCreatePartyWidget::OnBtn_AddNumClicked()
{
	MaxNum++;

	if (MaxNum > 4)
	{
		MaxNum = 4;
	}

	TextBlock_PartyMaxNum->SetText(FText::AsNumber(MaxNum));
}

void UCreatePartyWidget::OnBtn_CreatePartyClicked()
{
	
}

void UCreatePartyWidget::OnBtn_CloseCreatePartyClicked()
{
	// Cancel all task

	NPCFindParty->CloseCreatePartyWidget();
}

void UCreatePartyWidget::SetNPCShopCharacter(ANPCFindParty* NewNPCFindParty)
{
	NPCFindParty = NewNPCFindParty;
}
