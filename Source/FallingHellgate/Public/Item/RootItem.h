// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "ItemInfo.h"
#include "RootItem.generated.h"

UCLASS()
class FALLINGHELLGATE_API ARootItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARootItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Component)
	TObjectPtr<class USphereComponent> SphereCollision;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Component)
	TObjectPtr<UStaticMeshComponent> ItemMesh;

protected:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventInteraction();

	virtual void EventInteraction_Implementation() override;
};
