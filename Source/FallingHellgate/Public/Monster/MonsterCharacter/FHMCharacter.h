// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterWeaponInterface.h"
#include "FHMCharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class FALLINGHELLGATE_API AFHMCharacter : public ACharacter, public IMonsterWeaponInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFHMCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Monster")
	AActor* TargetPatrol;

	UPROPERTY(VisibleAnywhere, Category = "Monster")
	class UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	UAnimMontage* Hitstun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Monster")
	TSubclassOf<class AWeapon> WeaponTypes;

	UPROPERTY(Replicated)
	AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Monster")
	bool bIsBossCheck;

	UPROPERTY(Replicated)
	int32 PhaseCheck;

	UPROPERTY()
	AActor* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")

	class UFHMonsterStateComponent* MonsterStateComponent;

public:
	UFUNCTION(Server, Reliable)
	void ReqTrigger();

	UFUNCTION(NetMulticast, Reliable)
	void ResTrigger();

	UFUNCTION(Server, Reliable)
	void AttackTrigger();

	UFUNCTION(Server, Reliable)
	void ReqPhaseChange();

	UFUNCTION(NetMulticast, Reliable)
	void ResPhaseChange();

	UFUNCTION(Server, Reliable)
	void PhaseChangeTrigger();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Monster")
	void OnUpdateHp(float CurHp, float MaxHp);

	void OnUpdateHp_Implementation(float CurHp, float MaxHp);

	UFUNCTION(Category = "Monster")
	virtual void BindPlayerState();

	UFUNCTION(Server, Reliable, Category = "Monster")
	virtual void S2CDoRagdoll();

	UFUNCTION(NetMulticast, Reliable, Category = "Monster")
	virtual void DoRagdoll();

	UFUNCTION(Server, Reliable, Category = "Monster")
	void S2CTakeBlood();

	UFUNCTION(NetMulticast, Reliable, Category = "Monster")
	void S2MTakeBlood();

	UFUNCTION(Server, Reliable, Category = "Monster")
	void DestroyActor();

	UFUNCTION(Server, Reliable, Category = "Monster")
	void ReqEquipWeapon(TSubclassOf<class AWeapon> WeaponClass);

	UFUNCTION(Category = "Monster")
	void OnRep_Equip();

	UFUNCTION(Server, Reliable, Category = "Monster")
	void S2CTakeGroggy();

	UFUNCTION(Client, Reliable, Category = "Monster")
	virtual void S2COnGroggyEnded(UAnimMontage* Montage, bool bInterrupted);


	UFUNCTION(NetMulticast, Reliable, Category = "Monster")
	void TakeGroggy();

	UFUNCTION(NetMulticast, Reliable, Category = "Monster")
	virtual void OnGroggyEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(Server, Reliable, Category = "Monster")
	void S2CStageClear();

	UFUNCTION(NetMulticast, Reliable, Category = "Monster")
	void S2MStageClear();

	UFUNCTION(Client, Reliable, Category = "Monster")
	void S2CDropItem();

	UPROPERTY(Replicated)
	ACharacter* OwnChar;
	
	UPROPERTY(Replicated)
	bool bIsGroggy = false;

	UPROPERTY(Replicated)
	bool IsRagdoll;

	bool bHasDown;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ADropItem> DropItemClass;

public:

	FTimerHandle th_BindPlayerState;

	FTimerHandle th_DestroyActor;

	FTimerHandle th_AIState;

public:
	UFUNCTION(Category = "Monster")
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> StageClearWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	UUserWidget* StageClearWidget = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	class UNiagaraSystem* BloodEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	TSubclassOf<class ADecalActor> BloodDecalClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	class UMaterialInterface* BodyBloodDecalMat;

};
