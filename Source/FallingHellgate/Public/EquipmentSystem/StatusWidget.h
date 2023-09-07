// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
protected:
	void BindStatusCompEvents();

	FTimerHandle InitTimerHandle;

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Health;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Stamina;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Attack;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_AttackSpeed;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Critical;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Defence;

protected:
	UFUNCTION()
	void OnStatusUpdate(int32 UpdateHealth, int32 UpdateStamina, int32 UpdateAttack, float UpdateAttackSpeed, float UpdateCritical, int32 UpdateDefence);

};
