// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FHHUD.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API AFHHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<class UHUDWidget> HUDWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = Widget)
	class UHUDWidget* HUDWidget;

public:
	UFUNCTION(BlueprintCallable, Category = Widget)
	class UHUDWidget* GetHUDWidget();
};
