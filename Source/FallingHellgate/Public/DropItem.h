// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "DropItem.generated.h"

UCLASS()
class FALLINGHELLGATE_API ADropItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<class USphereComponent> RootSphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<UParticleSystemComponent> ItemParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<class USphereComponent> OverlapCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<class UWidgetComponent> LootWidgetComp;

private:
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, Category = Widget, Meta = (AllowPrivateAccess = true))
	float WidgetVisiblityRange{ 1500.f };

	FTimerHandle UpdateWidgetVisiblityTimerHandle;

	bool bWidgetVisibility;

protected:
	void UpdateWidgetVisiblity();

	bool IsCanSeePlayer();

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interface)
	void EventInteraction(ACharacter* OwnCharacter);

	virtual void EventInteraction_Implementation(ACharacter* OwnCharacter) override;

};
