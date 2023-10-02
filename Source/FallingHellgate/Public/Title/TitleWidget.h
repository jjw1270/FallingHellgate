// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* EditableTextBox_NickName;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GameStart;

	UPROPERTY(EditDefaultsOnly)
	FString LevelName;
	
protected:
	FText GetRandomNickName();

	UFUNCTION()
	void GameStart();

};
