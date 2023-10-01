// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCFindParty.h"
#include "FallingHellgate.h"
#include "NPCWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

void ANPCFindParty::EventInteraction_Implementation(ACharacter* OwnCharacter)
{
	Super::EventInteraction_Implementation(OwnCharacter);

	if (!NPCWidget)
	{
		return;
	}

	NPCWidget->SetNPCNameText(NPCName);
	NPCWidget->SetNPCDialoguesText(NPCDialogues);
	NPCWidget->SetMenu(1, NPCMenu1Text);
	NPCWidget->Btn_NPCMenu1->OnClicked.AddDynamic(this, &ANPCFindParty::OpenCreatePartyWidget);
	NPCWidget->SetMenu(2, NPCMenu2Text);
	NPCWidget->Btn_NPCMenu2->OnClicked.AddDynamic(this, &ANPCFindParty::OpenFindPartyWidget);

	NPCWidget->HideUnBoundMenu();
}

void ANPCFindParty::OpenCreatePartyWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("OpenCreatePartyWidget"));
}

void ANPCFindParty::OpenFindPartyWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("OpenFindPartyWidget"));

}

void ANPCFindParty::CloseCreatePartyWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("CloseCreatePartyWidget"));

}

void ANPCFindParty::CloseFindPartyWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("CloseFindPartyWidget"));

}
