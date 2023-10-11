// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyInfoWidget.h"
#include "FHPlayerController.h"
#include "FallingHellgate.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "HUDWidget.h"
#include "PlayerStatusComponent.h"
#include "PartyMemberInfoWidget.h"

void UPartyInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind UI Drag Event
	UIDragBtn->OnPressed.AddDynamic(this, &UPartyInfoWidget::OnDragBtnPressed);
	UIDragBtn->OnReleased.AddDynamic(this, &UPartyInfoWidget::OnDragBtnReleased);

}

void UPartyInfoWidget::SetPartyMember(const FText& MemberName, UPlayerStatusComponent* PlayerStatusComp, bool bRegist)
{
	if (bRegist)
	{
		for (UPartyMemberInfoWidget* PartyMemberInfoWidget : PartyMemberInfoWidgetArray)
		{
			if (PartyMemberInfoWidget->GetPartyMemberName().EqualTo(MemberName))
			{
				return;
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("%s"), *MemberName.ToString());

		CHECK_VALID(PlayerStatusComp);
		CHECK_VALID(PartyMemberInfoWidgetClass);
		UPartyMemberInfoWidget* NewPartyMemberInfoWidget = CreateWidget<UPartyMemberInfoWidget>(GetOwningPlayer(), PartyMemberInfoWidgetClass);
		NewPartyMemberInfoWidget->InitPartyMemberInfoWidget(MemberName, PlayerStatusComp);
		PartyMemberInfoWidgetArray.Add(NewPartyMemberInfoWidget);
		VerticalBox_MemberList->AddChild(NewPartyMemberInfoWidget);
	}
	else
	{
		for (UPartyMemberInfoWidget* PartyMemberInfoWidget : PartyMemberInfoWidgetArray)
		{
			if (PartyMemberInfoWidget->GetPartyMemberName().EqualTo(MemberName))
			{
				PartyMemberInfoWidget->RemoveFromParent();
				PartyMemberInfoWidgetArray.Remove(PartyMemberInfoWidget);

				break;
			}
		}
	}
}

void UPartyInfoWidget::OnDragBtnPressed()
{
	if (!HUDWidget)
	{
		AFHPlayerController* PC = GetOwningPlayer<AFHPlayerController>();
		CHECK_VALID(PC);
		HUDWidget = PC->GetHUDWidget();
	}

	HUDWidget->WidgetDragStart(this);
}

void UPartyInfoWidget::OnDragBtnReleased()
{
	CHECK_VALID(HUDWidget);

	HUDWidget->WidgetDragEnd();
}
