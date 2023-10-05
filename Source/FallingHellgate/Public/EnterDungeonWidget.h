// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnterDungeonWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UEnterDungeonWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_DungeonName;
	
public:
	void SetDungeonName(FString& NewDungeonName);
};
