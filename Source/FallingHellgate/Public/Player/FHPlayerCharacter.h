// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FHPlayerCharacter.generated.h"

class UModularSkeletalMeshComponent;

// Delegate called when Cloak Visibility button Pressed
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
					<Weapon_L> (Reserved)
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

	// Network
public:
	UFUNCTION(Server, Reliable)
	void Req_Attack(int NormalAttackCount, bool bIsSmash);

	UFUNCTION(Server, Reliable)
	void Req_PickUp(FRotator LookAtRot);

	UFUNCTION(Server, Reliable)
	void Req_UpdateCurrentHealth(int32 DefaultHP, int32 CurHP);

protected:
	UFUNCTION(Server, Reliable)
	void Req_Dash();

	UFUNCTION(NetMulticast, Reliable)
	void Res_Dash();

	UFUNCTION(NetMulticast, Reliable)
	void Res_Attack(int NormalAttackCount, bool bIsSmash);

	UFUNCTION(NetMulticast, Reliable)
	void Res_PickUp(FRotator LookAtRot);

	UFUNCTION(Server, Reliable)
	void Req_OnWeaponUpdate(const FWeaponItemData UpdateWeaponItemData, const bool bIsEquip);

	UFUNCTION(NetMulticast, Reliable)
	void Res_OnWeaponUpdate(const FWeaponItemData UpdateWeaponItemData, const bool bIsEquip);

	UFUNCTION(Server, Reliable)
	void Req_OnArmorUpdate(const EArmorType UpdateArmorType, const FArmorItemData UpdateArmorItemData, const bool bIsEquip);

	UFUNCTION(NetMulticast, Reliable)
	void Res_OnArmorUpdate(const EArmorType UpdateArmorType, const FArmorItemData UpdateArmorItemData, const bool bIsEquip);

	UFUNCTION(Server, Reliable)
	void Req_OnEquipVisibilityUpdate(const EArmorType UpdateArmorType);

	UFUNCTION(NetMulticast, Reliable)
	void Res_OnEquipVisibilityUpdate(const EArmorType UpdateArmorType);

	UFUNCTION(NetMulticast, Reliable)
	void Res_UpdateCurrentHealth(int32 DefaultHP, int32 CurHP);

public:
	FORCEINLINE class UPlayerStatusComponent* GetPlayerStatusComp() const { return PlayerStatusComp; }

	// Montages
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> DashMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> LootingMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> NormalAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> SmashAttackMontage;

protected:
	UPROPERTY()
	class UQuickSlotComponent* QuickSlotComp;

public:
	UPROPERTY(BlueprintReadOnly, Category = Interaction)
	AActor* InteractingActor;  // For AnimNotify on Interaction Montage Playing, Delete when Notify End Execution.

	UPROPERTY(BlueprintReadWrite, Category = Interaction)
	AActor* LootingEffect;

	UPROPERTY(BlueprintAssignable, Category = Event)
	FDele_Multi_EquipVisibilityUpdate EquipVisibilityUpdateDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 DashStamina{ 100 };

	UPROPERTY(EditDefaultsOnly)
	int32 NormalAttackStamina{ 80 };

	UPROPERTY(EditDefaultsOnly)
	int32 SmashAttackStamina{ 150 };

	bool bIsDash;

};
