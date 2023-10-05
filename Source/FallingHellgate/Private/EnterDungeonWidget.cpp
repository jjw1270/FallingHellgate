// Fill out your copyright notice in the Description page of Project Settings.


#include "EnterDungeonWidget.h"
#include "Components/TextBlock.h"

void UEnterDungeonWidget::SetDungeonName(FString& NewDungeonName)
{
	TextBlock_DungeonName->SetText(FText::FromString(NewDungeonName));
}
