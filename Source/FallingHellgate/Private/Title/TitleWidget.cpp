// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "FHGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_GameStart->OnClicked.AddDynamic(this, &UTitleWidget::GameStart);

	EditableTextBox_NickName->SetText(GetRandomNickName());
}

FText UTitleWidget::GetRandomNickName()
{
	FRandomStream Stream(FMath::Rand());

	int32 RandomIdx = Stream.RandRange(1, 9999);

	FString TempNickName = FString::Printf(TEXT("Player%d"), RandomIdx);

	return FText::FromString(TempNickName);
}

void UTitleWidget::GameStart()
{
	UFHGameInstance* GI = GetGameInstance<UFHGameInstance>();
	if (!GI)
	{
		return;
	}

	FText NickName = EditableTextBox_NickName->GetText();
	GI->SetPlayerName(NickName);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName), true);
}
