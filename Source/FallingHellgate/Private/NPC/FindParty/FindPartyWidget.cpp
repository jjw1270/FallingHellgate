// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPartyWidget.h"
#include "Components/Button.h"
#include "NPCFindParty.h"

void UFindPartyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Refresh->OnClicked.AddDynamic(this, &UFindPartyWidget::OnBtn_RefreshClicked);
	Btn_CloseFindParty->OnClicked.AddDynamic(this, &UFindPartyWidget::OnBtn_CloseFindPartyClicked);

}

void UFindPartyWidget::OnBtn_RefreshClicked()
{


}

void UFindPartyWidget::OnBtn_CloseFindPartyClicked()
{
	NPCFindParty->CloseFindPartyWidget();
}

void UFindPartyWidget::SetNPCShopCharacter(ANPCFindParty* NewNPCFindParty)
{
	NPCFindParty = NewNPCFindParty;
}
