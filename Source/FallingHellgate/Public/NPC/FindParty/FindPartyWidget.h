// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FindPartyWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UFindPartyWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrollBox_FindParty;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Refresh;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CloseFindParty;

protected:
	UPROPERTY()
	class ANPCFindParty* NPCFindParty;

protected:
	UFUNCTION()
	void OnBtn_RefreshClicked();

	UFUNCTION()
	void OnBtn_CloseFindPartyClicked();

public:
	void SetNPCShopCharacter(class ANPCFindParty* NewNPCFindParty);

};
