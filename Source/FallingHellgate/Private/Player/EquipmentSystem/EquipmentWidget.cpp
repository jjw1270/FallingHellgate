// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentWidget.h"
#include "FallingHellgate.h"
#include "FHPlayerController.h"
#include "FHPlayerCharacter.h"
#include "EquipmentComponent.h"

#include "Components/Button.h"
#include "FHHUD.h"
#include "HUDWidget.h"

void UEquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UIDragBtn->OnPressed.AddDynamic(this, &UEquipmentWidget::OnDragBtnPressed);
	UIDragBtn->OnReleased.AddDynamic(this, &UEquipmentWidget::OnDragBtnReleased);

	Btn_ShowHelmet->OnClicked.AddDynamic(this, &UEquipmentWidget::OnBtn_ShowHelmetClicked);
	Btn_ShowCloak->OnClicked.AddDynamic(this, &UEquipmentWidget::OnBtn_ShowCloakClicked);

	Btn_ShowHelmet->SetIsEnabled(false);
	Btn_ShowCloak->SetIsEnabled(false);

	BindEquipmentCompEvents();
}

void UEquipmentWidget::BindEquipmentCompEvents()
{
	AFHPlayerController* PC = GetOwningPlayer<AFHPlayerController>();

	if (IsValid(PC))
	{
		UEquipmentComponent* EquipComp = PC->GetEquipmentComp();
		if (IsValid(EquipComp))
		{
			EquipComp->ArmorUpdateDelegate.AddUObject(this, &UEquipmentWidget::OnArmorUpdate);
			return;
		}
	}

	GetWorld()->GetTimerManager().SetTimer(InitTimerHandle, this, &UEquipmentWidget::BindEquipmentCompEvents, 0.1f, false);
}

void UEquipmentWidget::OnDragBtnPressed()
{
	if (!HUDWidget)
	{
		AFHHUD* HUD = Cast<AFHHUD>(GetOwningPlayer()->GetHUD());
		CHECK_VALID(HUD);

		HUDWidget = HUD->GetHUDWidget();
	}

	HUDWidget->WidgetDragStart(this);
}

void UEquipmentWidget::OnDragBtnReleased()
{
	CHECK_VALID(HUDWidget);

	HUDWidget->WidgetDragEnd();
}

void UEquipmentWidget::OnArmorUpdate(const EArmorType& UpdateArmorType, const int32& UpdateItemID, const bool& bEquip)
{
	if (UpdateArmorType == EArmorType::Helmet)
	{
		Btn_ShowHelmet->SetIsEnabled(bEquip);
		return;
	}

	if (UpdateArmorType == EArmorType::Upper)
	{
		Btn_ShowCloak->SetIsEnabled(bEquip);
		return;
	}
}

void UEquipmentWidget::OnBtn_ShowHelmetClicked()
{
	AFHPlayerCharacter* PlayerChar = GetOwningPlayerPawn<AFHPlayerCharacter>();
	CHECK_VALID(PlayerChar);
	
	PlayerChar->EquipVisibilityUpdateDelegate.Broadcast(EArmorType::Helmet);
}

void UEquipmentWidget::OnBtn_ShowCloakClicked()
{
	AFHPlayerCharacter* PlayerChar = GetOwningPlayerPawn<AFHPlayerCharacter>();
	CHECK_VALID(PlayerChar);

	PlayerChar->EquipVisibilityUpdateDelegate.Broadcast(EArmorType::Upper);
}
