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
	class UButton* Btn_Refresh;

	UPROPERTY(meta = (BindWidget))
	class UBackgroundBlur* BackgroundBlur_FindParty;

protected:
	UFUNCTION()
	void OnBtn_RefreshClicked();

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FindParty();
	virtual void FindParty_Implementation();

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FindPartyComplete(const TArray<FBlueprintSessionResult>& SessionResult);
	virtual void FindPartyComplete_Implementation(const TArray<FBlueprintSessionResult>& SessionResult);

};
