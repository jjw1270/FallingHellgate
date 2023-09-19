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

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

protected:
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	virtual void Move(const FInputActionValue& Value);   //Temp Virtual for develop

	/** Called for looking input */
	virtual void Look(const FInputActionValue& Value);   //Temp Virtual for develop

public:
	// Use When Replicate Move Server and Client
	UPROPERTY(Replicated)
	FRotator PlayerRotation;

	//Return Player index 0's Rotation or just PlayerRotation
	UFUNCTION(BlueprintPure)
	FRotator GetPlayerRotation();

/*
Modular Mesh Components
<LowerBody(Mesh)>
	<Shoes>
	<UpperBody>
		<Cloak>
		<Glove_L>
			<Weapon_L>
		<Glove_R>
			<Weapon_R>
		<Head>
			<Hair>
			<Helmet>
*/ 
protected:
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
	UModularSkeletalMeshComponent* Weapon;

	UPROPERTY()
	TArray<UModularSkeletalMeshComponent*> ArmorMSMCompArray;

protected:
	void InitModularMeshComp();

// Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Component)
	TObjectPtr<UCapsuleComponent> InteractCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component)
	class UPlayerStatusComponent* PlayerStatusComp;

protected:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	/** Roll Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RollAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	/** RightClick Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RightClickAction;

	/** LeftClick Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LeftClickAction;

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

protected:
	/** Called for Roll input */
	void RollInput(const FInputActionValue& Value);

	/** Called for Sprint input */
	void SprintInput(const FInputActionValue& Value);

	/** Called for StopSprint input */
	void StopSprintInput(const FInputActionValue& Value);

	/** Called for RightClick input */
	void RightClickInput(const FInputActionValue& Value);

	/** Called for StopRightClick input */
	void StopRightClickInput(const FInputActionValue& Value);

	/** Called for LeftClick input */
	void LeftClickInput(const FInputActionValue& Value);

	/** Called for StopLeftClick input */
	void StopLeftClickInput(const FInputActionValue& Value);

	/** Called for Interact input */
	void Interaction(const FInputActionValue& Value);

	/** Called for Quick Slot inputs */
	void UseQuickSlot(int32 SlotNum);

protected:
	UFUNCTION()
	void OnWeaponUpdate(class UItemData* UpdateEquipItem, const bool& bIsEquip);

	UFUNCTION()
	void OnArmorUpdate(const EArmorType& UpdateArmorType, class UItemData* UpdateEquipItem, const bool& bIsEquip);

	UFUNCTION()
	void OnEquipVisibilityUpdate(const EArmorType& UpdateArmorType);

public:
	// Do Roll Move function
	UFUNCTION(Server, Reliable)
	void Req_DoRollMove();

	UFUNCTION(NetMulticast, Reliable)
	void Res_DoRollMove();

	// Set MaxWalkSpeed For Sprint Action
	UFUNCTION(Server, Reliable)
	void Req_SetMaxWalkSpeed(float NewSpeed);

	UFUNCTION(NetMulticast, Reliable)
	void Res_SetMaxWalkSpeed(float NewSpeed);

	//Left Click Attack Action
	UFUNCTION(Server, Reliable)
	void Req_LeftClickAttack(bool IsPressed);

	UFUNCTION(NetMulticast, Reliable)
	void Res_LeftClickAttack(bool IsPressed);

	void LeftClickAttack(bool IsPressed);

	//Right Click Attack Action
	UFUNCTION(Server, Reliable)
	void Req_RightClickAttack(bool IsPressed);

	UFUNCTION(NetMulticast, Reliable)
	void Res_RightClickAttack(bool IsPressed);

	void RightClickAttack(bool IsPressed);

	UFUNCTION(Server, Reliable)
	void Req_PickUp(FRotator LookAtRot);

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

public:
	// Check Any MontagePlaying
	UFUNCTION(BlueprintCallable)
	bool bIsMontagePlaying() { return GetMesh()->GetAnimInstance()->IsAnyMontagePlaying(); };



protected:
	// Use When Roll
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	class UAnimMontage* StandToRollMontage;

	// Use When Roll
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	class UAnimMontage* RunToRollMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Montage)
	TObjectPtr<class UAnimMontage> LootingMontage;

protected:
	UPROPERTY()
	class AFHPlayerController* PC;

	UPROPERTY()
	class UQuickSlotComponent* QuickSlotComp;

public:
	UPROPERTY(BlueprintReadOnly, Category = Interaction)
	AActor* InteractingActor;  // For AnimNotify on Interaction Montage Playing, Delete when Notify End Execution.

	UPROPERTY(BlueprintReadWrite, Category = Interaction)
	AActor* LootingEffect;

	UPROPERTY(BlueprintAssignable, Category = Event)
	FDele_Multi_EquipVisibilityUpdate EquipVisibilityUpdateDelegate;

};
