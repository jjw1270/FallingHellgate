// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnDragWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UOnDragWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

	void SetItemImage(class UTexture2D* Image);
};
