// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCShopCharacter.h"
#include "NPCWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/Widget.h"

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

	NPCWidget->HideUnBoundMenu();
}

void ANPCShopCharacter::OpenShop()
{
	UE_LOG(LogTemp, Warning, TEXT("SHOPPING"));
	if (UWidget* NPCDefaultWidget = Cast<UWidget>(NPCWidget->Border_NPCDefault))
	{
		NPCDefaultWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ANPCShopCharacter::CloseShop()
{

}
