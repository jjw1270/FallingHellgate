// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterWeaponInterface.h"
#include "Weapon.generated.h"

UCLASS()
class FALLINGHELLGATE_API AWeapon : public AActor, public IMonsterWeaponInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterWeapon")
	class UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<class UAnimMontage*> AttackAnimations;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* PhaseChangeAnim1;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* PhaseChangeAnim2;

	UPROPERTY()
	class UAnimMontage* WeaponMontage;

	UPROPERTY(Replicated)
	ACharacter* OwnChar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "MonsterWeapon")
	float Range;

	UPROPERTY(Replicated)
	TArray<AActor*> FHMCharactersToIgnore;

	UFUNCTION(Server, Reliable, Category = "MonsterWeapon")
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float Damage;

	UPROPERTY(Replicated)
	bool bIsMontagePlaying;

public:
	UFUNCTION(Server, Reliable, Category = "MonsterWeapon")
	void PlayAttackMontage();

	UFUNCTION(NetMulticast, Reliable, Category = "MonsterWeapon")
	void PlayAnimation(int32 Index);

	UFUNCTION(Server, Reliable, Category = "MonsterWeapon")
	void PlayPhaseChangeMontage();

	UFUNCTION(NetMulticast, Reliable, Category = "MonsterWeapon")
	void PlayPhaseChangeAnimation();

	UFUNCTION(Server, Reliable, Category = "MonsterWeapon")
	void EventAttack();

	virtual void EventAttack_Implementation() override;

	UFUNCTION(Server, Reliable, Category = "MonsterWeapon")
	void EventTrigger();

	virtual void EventTrigger_Implementation() override;

	UFUNCTION(Server, Reliable, Category = "MonsterWeapon")
	void EventPhase();

	virtual void EventPhase_Implementation() override;

};
