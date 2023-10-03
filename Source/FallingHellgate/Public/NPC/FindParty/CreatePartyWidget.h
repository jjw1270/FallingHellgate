// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreatePartyWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UCreatePartyWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* EditableTextBox_PartyName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_PartyMaxNum;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_SubNum;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_AddNum;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CreateParty;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CloseCreateParty;

protected:
	int32 MaxNum;

	UPROPERTY()
	class ANPCFindParty* NPCFindParty;

protected:
	UFUNCTION()
	void OnBtn_SubNumClicked();

	UFUNCTION()
	void OnBtn_AddNumClicked();

	UFUNCTION()
	void OnBtn_CreatePartyClicked();

	UFUNCTION()
	void OnBtn_CloseCreatePartyClicked();

public:
	void SetNPCShopCharacter(class ANPCFindParty* NewNPCFindParty);

};
