// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UPlayerStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	void BindStatusCompEvents();

protected:
	UPROPERTY()
	class UPlayerStatusComponent* PlayerStatusComp;

	FTimerHandle InitTimerHandle;

	UPROPERTY(BlueprintReadOnly, Category = Widget)
	class UHUDWidget* HUDWidget;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* UIDragBtn;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_PlayerName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_MaxHP;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_CurHP;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_MaxStamina;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_CurStamina;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_HP;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Stamina;

protected:
	FVector2D MousePosOnDragStart;

	FTimerHandle DragTimerHandle;

	int32 CurHP;
	int32 DefaultHP;
	int32 CurStamina;
	int32 DefaultStamina;

// Bind Events
protected:
	UFUNCTION()
	void OnDefaultStatusUpdated(int32 UpdateHealth, int32 UpdateStamina, int32 UpdateAttack, float UpdateAttackSpeed, float UpdateCritical, int32 UpdateDefence);

	UFUNCTION()
	void OnCurrentStatusUpdated(int32 UpdateHealth, int32 UpdateStamina, int32 UpdateAttack, float UpdateAttackSpeed, float UpdateCritical, int32 UpdateDefence);

	void UpdateHPProgressBar();

	void UpdateStaminaProgressBar();

//Drag Func
protected:
	UFUNCTION()
	void OnDragBtnPressed();

	UFUNCTION()
	void OnDragBtnReleased();

};