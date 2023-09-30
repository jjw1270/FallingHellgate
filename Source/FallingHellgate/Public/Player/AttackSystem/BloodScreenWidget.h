// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BloodScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UBloodScreenWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowBloodScreen(bool bIsDead);
};
