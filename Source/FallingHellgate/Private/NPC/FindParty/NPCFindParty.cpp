// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCFindParty.h"
#include "FallingHellgate.h"
#include "NPCWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/OverlaySlot.h"
#include "Components/Overlay.h"
#include "CreatePartyWidget.h"
#include "FindPartyWidget.h"

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
	if (UWidget* NPCDefaultWidget = Cast<UWidget>(NPCWidget->Border_NPCDefault))
	{
		NPCDefaultWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	CHECK_VALID(CreatePartyWidgetClass);
	CreatePartyWidget = CreateWidget<UCreatePartyWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), CreatePartyWidgetClass);
	CHECK_VALID(CreatePartyWidget);
	CreatePartyWidget->SetNPCShopCharacter(this);

	UOverlaySlot* OS_CreatePartyWidget = NPCWidget->Overlay_NPCDefault->AddChildToOverlay(CreatePartyWidget);
	CHECK_VALID(OS_CreatePartyWidget);
	OS_CreatePartyWidget->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	OS_CreatePartyWidget->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
}

void ANPCFindParty::OpenFindPartyWidget()
{
	if (UWidget* NPCDefaultWidget = Cast<UWidget>(NPCWidget->Border_NPCDefault))
	{
		NPCDefaultWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	CHECK_VALID(FindPartyWidgetClass);
	FindPartyWidget = CreateWidget<UFindPartyWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), FindPartyWidgetClass);
	CHECK_VALID(FindPartyWidget);
	FindPartyWidget->SetNPCShopCharacter(this);

	UOverlaySlot* OS_FindPartyWidget = NPCWidget->Overlay_NPCDefault->AddChildToOverlay(FindPartyWidget);
	CHECK_VALID(OS_FindPartyWidget);
	OS_FindPartyWidget->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	OS_FindPartyWidget->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
}

void ANPCFindParty::CloseCreatePartyWidget()
{
	CHECK_VALID(CreatePartyWidget);
	CreatePartyWidget->RemoveFromParent();
	CreatePartyWidget = nullptr;

	if (UWidget* NPCDefaultWidget = Cast<UWidget>(NPCWidget->Border_NPCDefault))
	{
		NPCDefaultWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ANPCFindParty::CloseFindPartyWidget()
{
	CHECK_VALID(FindPartyWidget);
	FindPartyWidget->RemoveFromParent();
	FindPartyWidget = nullptr;

	if (UWidget* NPCDefaultWidget = Cast<UWidget>(NPCWidget->Border_NPCDefault))
	{
		NPCDefaultWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
