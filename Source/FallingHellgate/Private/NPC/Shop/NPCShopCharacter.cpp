// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCShopCharacter.h"
#include "FallingHellgate.h"
#include "NPCWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/Widget.h"
#include "Components/Overlay.h"
#include "ShopWidget.h"
#include "Components/OverlaySlot.h"

#include "Kismet/GameplayStatics.h"

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

	CHECK_VALID(ShopWidgetClass);
	ShopWidget = CreateWidget<UShopWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), ShopWidgetClass);
	CHECK_VALID(ShopWidget);
	UOverlaySlot* OS_ShopWidget = NPCWidget->Overlay_NPCDefault->AddChildToOverlay(ShopWidget);
	CHECK_VALID(OS_ShopWidget);
	OS_ShopWidget->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	OS_ShopWidget->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
}

void ANPCShopCharacter::CloseShop()
{
	CHECK_VALID(ShopWidget);
	ShopWidget->RemoveFromParent();
}
