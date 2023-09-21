// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "ModularSkeletalMeshComponent.h"
#include "BaseWeapon.generated.h"

UCLASS()
class FALLINGHELLGATE_API UBaseWeapon : public UModularSkeletalMeshComponent, public IWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UBaseWeapon(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//Interface Event
public:
	// Normal Attack
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventNormalAttack(ACharacter* OwnCharacter);

	virtual void EventNormalAttack_Implementation(ACharacter* OwnCharacter) override;

	// Smash Attack
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EventSmashAttack(ACharacter* OwnCharacter);

	virtual void EventSmashAttack_Implementation(ACharacter* OwnCharacter) override;

protected:
	//----------[ Value ]----------
	//Add Count When Completed Left Click Attack, Reset Count When Right Click Attack
	int32 NormalAttackCount;

	int32 MaxNormalAttackCount{ 4 };

	bool bCanAttack;

//Attack Collision
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision)
	FName AttackStartSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision)
	FName AttackEndSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision)
	float TraceSphereRadius;

public:
	FORCEINLINE void SetCanAttack(bool bNewCanAttack);

	FORCEINLINE bool CanAttack() const { return bCanAttack; }

	//Return LeftClickCount
	FORCEINLINE int32 GetNormalAttackCount() const { return NormalAttackCount; }

	//Return MaxLeftClickCount
	FORCEINLINE int32 GetMaxNormalAttackCount() const { return MaxNormalAttackCount; }

	//Initialize LeftClickCount
	void ResetNormalAttackCount();

public:
	void Attack(ACharacter* OwnCharacter, bool bIsSmash);

};
