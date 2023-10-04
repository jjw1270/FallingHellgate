// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnterDungeon.generated.h"

UCLASS()
class FALLINGHELLGATE_API AEnterDungeon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnterDungeon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
