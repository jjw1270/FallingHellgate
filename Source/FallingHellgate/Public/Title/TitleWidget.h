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

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	class UButton* Btn_CreateParty;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	class UButton* Btn_FindParty;
	
protected:
	FText GetRandomNickName();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnBtn_CreatePartyClicked();
	virtual void OnBtn_CreatePartyClicked_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnBtn_FindPartyClicked();
	virtual void OnBtn_FindPartyClicked_Implementation();

};
