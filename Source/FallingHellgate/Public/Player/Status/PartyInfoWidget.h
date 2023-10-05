// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PartyInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UPartyInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Widget)
	class UHUDWidget* HUDWidget;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* UIDragBtn;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* VerticalBox_MemberList;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UPartyMemberInfoWidget> PartyMemberInfoWidgetClass;
	
	UPROPERTY()
	TArray<class UPartyMemberInfoWidget*> PartyMemberInfoWidgetArray;

public:
	UFUNCTION(BlueprintCallable)
	void SetPartyMember(const FText& MemberName, class UPlayerStatusComponent* PlayerStatusComp, bool bRegist);

//Drag Func
protected:
	UFUNCTION()
	void OnDragBtnPressed();

	UFUNCTION()
	void OnDragBtnReleased();

};
