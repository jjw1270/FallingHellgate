// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FHPlayerCharacter.generated.h"

class UModularSkeletalMeshComponent;

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Front		UMETA(DisplayName = "Front"),
	Back		UMETA(DisplayName = "Back"),
	Left		UMETA(DisplayName = "Left"),
	Right		UMETA(DisplayName = "Right")
};

// Delegate called when Cloak Visibility button PS2Csed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Multi_EquipVisibilityUpdate, const EArmorType&, UpdateArmorType);

UCLASS()
class FALLINGHELLGATE_API AFHPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFHPlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	//[Modular Mesh Components]
	/*	<LowerBody(Mesh)>
			<Shoes>
			<UpperBody>
				<Cloak>
				<Glove_L>
					<Weapon_L> (S2Cerved)
				<Glove_R>
					<Weapon_R>
				<Head>
					<Hair>
					<Helmet> */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeshComponent)
	UModularSkeletalMeshComponent* Shoes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeshComponent)
	UModularSkeletalMeshComponent* UpperBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeshComponent)
	UModularSkeletalMeshComponent* Cloak;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeshComponent)
	UModularSkeletalMeshComponent* Glove_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeshComponent)
	UModularSkeletalMeshComponent* Glove_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeshComponent)
	UModularSkeletalMeshComponent* Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeshComponent)
	UModularSkeletalMeshComponent* Hair;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeshComponent)
	UModularSkeletalMeshComponent* Helmet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeshComponent)
	class UBaseWeapon* Weapon;

	UPROPERTY()
	TArray<UModularSkeletalMeshComponent*> ArmorMSMCompArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<UCapsuleComponent> InteractCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class UPlayerStatusComponent* PlayerStatusComp;

protected:
	void InitModularMeshComp();

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UBaseWeapon* GetWeapon() const { return Weapon; }

// Inputs
protected:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DashAction;

	/** NormalAttack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* NormalAttackAction;

	/** SmashAttack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SmashAttackAction;

	/** interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* InteractAction;

	// Quick Slot Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* QuickSlot1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* QuickSlot2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* QuickSlot3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* QuickSlot4Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* QuickSlot5Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputAction* QuickSlot6Action;

public:
	// Check Any MontagePlaying
	UFUNCTION(BlueprintCallable)
	bool CanPlayMontage();

protected:
	virtual void Jump() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Dash input */
	void Dash(const FInputActionValue& Value);

	/** Called for NormalAttack input */
	void NormalAttack(const FInputActionValue& Value);

	/** Called for SmashAttack input */
	void SmashAttack(const FInputActionValue& Value);

	/** Called for Interact input */
	void Interaction(const FInputActionValue& Value);

	/** Called for Quick Slot inputs */
	void UseQuickSlot(int32 SlotNum);

	// Delegate Binding Events
protected:
	UFUNCTION()
	void OnWeaponUpdate(class UItemData* UpdateEquipItem, const bool& bIsEquip);

	UFUNCTION()
	void OnArmorUpdate(const EArmorType& UpdateArmorType, class UItemData* UpdateEquipItem, const bool& bIsEquip);

	UFUNCTION()
	void OnEquipVisibilityUpdate(const EArmorType& UpdateArmorType);

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Respawn();

// Network
public:
	UFUNCTION(Server, Reliable)
	void C2S_PickUp(const FRotator& LookAtRot);
	
	UFUNCTION(Server, Reliable)
	void C2S_Attack(const FRotator& AttackRot, class UAnimMontage* AttackMontage, const FName& SectionName, const float& AttackSpeed);

	void ApplyDamage(AActor* TargetActor, const float& DamageCoefficient);

	UFUNCTION(Server, Reliable)
	void C2S_PlayUseItemEffect(const EEffectTarget& EffectTarget);

protected:
	UFUNCTION(Server, Reliable)
	void C2S_Dash(const FRotator& DashRot);

	UFUNCTION(NetMulticast, Reliable)
	void S2M_Dash(const FRotator& DashRot);

	UFUNCTION(NetMulticast, Reliable)
	void S2M_PickUp(const FRotator& LookAtRot);

	UFUNCTION(NetMulticast, Reliable)
	void S2M_Attack(const FRotator& AttackRot, class UAnimMontage* AttackMontage, const FName& SectionName, const float& AttackSpeed);

	UFUNCTION(NetMulticast, Reliable)
	void S2M_ApplyDamage();

	UFUNCTION(Server, Reliable)
	void C2S_TakeDamage(const EHitDirection& HitDir, const bool bKnockBack);

	UFUNCTION(NetMulticast, Reliable)
	void S2M_TakeDamage(class UAnimMontage* HitReactMontage);

	UFUNCTION(Server, Reliable)
	void C2S_Death(const EHitDirection& HitDir);

	UFUNCTION(NetMulticast, Reliable)
	void S2M_Death(class UAnimMontage* DeathMontage);

	UFUNCTION(NetMulticast, Reliable)
	void S2M_Respawn();

	UFUNCTION(Server, Reliable)
	void C2S_UseItem(const int32& QuickSlotIdx);

	UFUNCTION(NetMulticast, Reliable)
	void S2M_UseItem();

	UFUNCTION(Client, Reliable)
	void S2C_UseItem(const int32& QuickSlotIdx);

	UFUNCTION(NetMulticast, Reliable)
	void S2M_PlayUseItemEffect(const EEffectTarget& EffectTarget);

	UFUNCTION(Server, Reliable)
	void C2S_OnWeaponUpdate(const FWeaponItemData& UpdateWeaponItemData, const bool bIsEquip);

	UFUNCTION(NetMulticast, Reliable)
	void S2M_OnWeaponUpdate(const FWeaponItemData& UpdateWeaponItemData, const bool bIsEquip);

	UFUNCTION(Server, Reliable)
	void C2S_OnArmorUpdate(const EArmorType& UpdateArmorType, const FArmorItemData& UpdateArmorItemData, const bool bIsEquip);

	UFUNCTION(NetMulticast, Reliable)
	void S2M_OnArmorUpdate(const EArmorType& UpdateArmorType, const FArmorItemData& UpdateArmorItemData, const bool bIsEquip);

	UFUNCTION(Server, Reliable)
	void C2S_OnEquipVisibilityUpdate(const EArmorType& UpdateArmorType);

	UFUNCTION(NetMulticast, Reliable)
	void S2M_OnEquipVisibilityUpdate(const EArmorType& UpdateArmorType);

public:
	FORCEINLINE class UPlayerStatusComponent* GetPlayerStatusComp() const { return PlayerStatusComp; }

	// Montages
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> DashMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> LootingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> HitReactFrontMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> HitReactBackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> HitReactLeftMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> HitReactRightMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> KnockBackFrontMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> KnockBackBackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> DeathFrontMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> DeathBackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> DrinkMontage;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	class UNiagaraSystem* BloodEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	TSubclassOf<class ADecalActor> BloodDecalClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	class UMaterialInterface* BodyBloodDecalMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	TSubclassOf<class UCameraShakeBase> ApplyDamageCameraShakeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	TArray<class USoundBase*> ApplyDamageSounds;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	class UParticleSystem* HealthParticleSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	class UParticleSystem* StaminaParticleSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	class UParticleSystem* AttackPowerParticleSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	class UParticleSystem* DefensivePowerParticleSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effect)
	class USoundBase* ApplyItemSound;

protected:
	UPROPERTY()
	class UQuickSlotComponent* QuickSlotComp;

public:
	UPROPERTY(BlueprintReadOnly, Category = Interaction)
	AActor* InteractingActor;  // For AnimNotify on Interaction Montage Playing, Delete when Notify End Execution.

	UPROPERTY(BlueprintReadWrite, Category = Interaction)
	AActor* LootingEffect;

	UPROPERTY()
	class UItemData* TempUseItem;

	UPROPERTY(BlueprintAssignable, Category = Event)
	FDele_Multi_EquipVisibilityUpdate EquipVisibilityUpdateDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 JumpStamina{ 100 };

	UPROPERTY(EditDefaultsOnly)
	int32 DashStamina{ 100 };

	UPROPERTY(EditDefaultsOnly)
	int32 NormalAttackStamina{ 40 };

	UPROPERTY(EditDefaultsOnly)
	int32 SmashAttackStamina{ 80 };

	bool bIsDashCool;

public:
	UPROPERTY(BlueprintReadWrite)
	bool bDodge;

};
