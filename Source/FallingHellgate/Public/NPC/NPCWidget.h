// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UNPCWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_NPCName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_NPCDialogues;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_NPCMenu1;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_NPCMenu2;

	UPROPERTY(meta = (BindWidget))
	class USpacer* Spacer_Menu1;

	UPROPERTY(meta = (BindWidget))
	class USpacer* Spacer_Menu2;

public:
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_NPCDefault;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Exit;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_NPCMenu1;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_NPCMenu2;

public:
	void SetNPCNameText(FText NPCName);

	void SetNPCDialoguesText(FText NPCDialogues);

	void HideUnBoundMenu();

	void SetMenu(int32 MenuIndex, FText MenuText);

};
