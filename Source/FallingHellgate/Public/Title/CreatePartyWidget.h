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
	class UTextBlock* TextBlock_PartyMaxNum;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_SubNum;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_AddNum;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CreateParty;

public:
	UPROPERTY(BlueprintReadWrite)
	int32 MaxNum;

protected:
	UFUNCTION()
	void OnBtn_SubNumClicked();

	UFUNCTION()
	void OnBtn_AddNumClicked();

	UFUNCTION(BlueprintNativeEvent)
	void OnBtn_CreatePartyClicked();
	virtual void OnBtn_CreatePartyClicked_Implementation();

};
