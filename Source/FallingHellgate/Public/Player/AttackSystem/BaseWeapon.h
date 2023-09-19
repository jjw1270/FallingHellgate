// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"


enum class EItemType : uint8;

UCLASS()
class FALLINGHELLGATE_API ABaseWeapon : public AActor, public IWeaponInterface
{
	GENERATED_BODY()
	

};
