// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartyMemberInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UPartyMemberInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_MemberName;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_HP;

public:
	void InitPartyMemberInfoWidget(const FText& NewMemberName, class UPlayerStatusComponent* PlayerStatusComp);
	
	FText GetPartyMemberName();

protected:
	UFUNCTION()
	void OnDefaultStatusUpdated(int32 UpdateHealth, int32 UpdateStamina, int32 UpdateAttack, float UpdateAttackSpeed, float UpdateCritical, int32 UpdateDefence);

	UFUNCTION()
	void OnCurrentStatusUpdated(int32 UpdateHealth, int32 UpdateStamina, int32 UpdateAttack, float UpdateAttackSpeed, float UpdateCritical, int32 UpdateDefence);

protected:
	int32 CurHP;
	int32 DefaultHP;
};
