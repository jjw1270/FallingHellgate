// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCShopCharacter.h"
#include "NPCWidget.h"
#include "Components/Button.h"

void ANPCShopCharacter::EventInteraction_Implementation(ACharacter* OwnCharacter)
{
	Super::EventInteraction_Implementation(OwnCharacter);

	if (!NPCWidget)
	{
		return;
	}

	NPCWidget->SetNPCNameText(NPCName);
	NPCWidget->SetNPCDialoguesText(NPCDialogues);
	NPCWidget->SetMenu(1, NPCMenu1Text);
	NPCWidget->Btn_NPCMenu1->OnClicked.AddDynamic(this, &ANPCShopCharacter::OpenShop);
}

void ANPCShopCharacter::OpenShop()
{

}
