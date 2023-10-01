// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/NPCBase.h"
#include "NPCFindParty.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API ANPCFindParty : public ANPCBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = NPC)
	FText NPCMenu1Text;

	UPROPERTY(EditAnywhere, Category = NPC)
	FText NPCMenu2Text;

	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<class UUserWidget> FindPartyWidgetClass;

	UPROPERTY()
	class UUserWidget* FindPartyWidget;
	
public:
	virtual void EventInteraction_Implementation(ACharacter* OwnCharacter) override;

protected:
	UFUNCTION()
	void OpenCreatePartyWidget();

	UFUNCTION()
	void OpenFindPartyWidget();

public:
	UFUNCTION()
	void CloseCreatePartyWidget();

	UFUNCTION()
	void CloseFindPartyWidget();

};
