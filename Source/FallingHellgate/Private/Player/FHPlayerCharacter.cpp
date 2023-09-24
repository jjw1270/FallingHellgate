// Fill out your copyright notice in the Description page of Project Settings.


#include "FHPlayerCharacter.h"
#include "FallingHellgate.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "QuickSlotComponent.h"
#include "EquipmentComponent.h"
#include "PlayerStatusComponent.h"

#include "InteractionInterface.h"
#include "WeaponInterface.h"

#include "FHPlayerController.h"
#include "ItemData.h"
#include "BaseWeapon.h"

AFHPlayerCharacter::AFHPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UModularSkeletalMeshComponent>(ACharacter::MeshComponentName = TEXT("LowerBody")))
{
	PrimaryActorTick.bCanEverTick = false;

	// Don't rotate when the Controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 350.f;
	GetCharacterMovement()->AirControl = 0.35f;
	//----------[ Default Speed Setting Here ]----------
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->TargetOffset = FVector(0.f, 0.f, 100.f);
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Set Modular Meshes
	InitModularMeshComp();

	// Interact Collision
	InteractCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(RootComponent);
	InteractCollision->SetCapsuleSize(60.f, 120.f);

	PlayerStatusComp = CreateDefaultSubobject<UPlayerStatusComponent>(TEXT("PlayerStatus"));
}

void AFHPlayerCharacter::InitModularMeshComp()
{
	UModularSkeletalMeshComponent* LowerBody = Cast<UModularSkeletalMeshComponent>(GetMesh());
	CHECK_VALID(LowerBody);
	LowerBody->SetArmorType(EArmorType::Lower);
	ArmorMSMCompArray.Add(LowerBody);

	Shoes = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Shoes"));
	Shoes->SetupAttachment(GetMesh());
	Shoes->SetArmorType(EArmorType::Shoes);
	Shoes->SetLeaderPoseComponent(GetMesh());
	ArmorMSMCompArray.Add(Shoes);

	UpperBody = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("UpperBody"));
	UpperBody->SetupAttachment(GetMesh());
	UpperBody->SetArmorType(EArmorType::Upper);
	UpperBody->SetLeaderPoseComponent(GetMesh());
	ArmorMSMCompArray.Add(UpperBody);

	Cloak = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Cloak"));
	Cloak->SetupAttachment(UpperBody);
	Cloak->SetArmorType(EArmorType::None);
	Cloak->SetLeaderPoseComponent(GetMesh());

	Glove_L = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Glove_L"));
	Glove_L->SetupAttachment(UpperBody);
	Glove_L->SetArmorType(EArmorType::Gloves);
	Glove_L->SetLeaderPoseComponent(GetMesh());
	ArmorMSMCompArray.Add(Glove_L);

	Glove_R = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Glove_R"));
	Glove_R->SetupAttachment(UpperBody);
	Glove_R->SetArmorType(EArmorType::None);
	Glove_R->SetLeaderPoseComponent(GetMesh());

	Head = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(UpperBody);
	Head->SetArmorType(EArmorType::None);
	Head->SetLeaderPoseComponent(GetMesh());

	Hair = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(Head);
	Hair->SetArmorType(EArmorType::None);
	Hair->SetLeaderPoseComponent(GetMesh());

	Helmet = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Helmet"));
	Helmet->SetupAttachment(Head, TEXT("HAIR"));
	Helmet->SetArmorType(EArmorType::Helmet);
	ArmorMSMCompArray.Add(Helmet);

	Weapon = CreateDefaultSubobject<UBaseWeapon>(TEXT("Weapon"));
	Weapon->SetupAttachment(Glove_R, TEXT("WEAPON_R"));
	Weapon->SetArmorType(EArmorType::None);
}

void AFHPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AFHPlayerController* PC = GetController<AFHPlayerController>();
	CHECK_VALID(PC);

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	QuickSlotComp = PC->GetQuickSlotComp();
	CHECK_VALID(QuickSlotComp);

	UEquipmentComponent* EquipmentComp = PC->GetEquipmentComp();
	CHECK_VALID(EquipmentComp);

	// Bind Equipment Update Delegates
	EquipmentComp->WeaponUpdateDelegate.AddUObject(this, &AFHPlayerCharacter::OnWeaponUpdate);
	EquipmentComp->ArmorUpdateDelegate.AddUObject(this, &AFHPlayerCharacter::OnArmorUpdate);

	EquipVisibilityUpdateDelegate.AddDynamic(this, &AFHPlayerCharacter::OnEquipVisibilityUpdate);
}

void AFHPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFHPlayerCharacter::Move);
		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFHPlayerCharacter::Look);
		//Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AFHPlayerCharacter::Dash);
		//Sprint
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AFHPlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		//NormalAttack
		EnhancedInputComponent->BindAction(NormalAttackAction, ETriggerEvent::Started, this, &AFHPlayerCharacter::NormalAttack);
		//SmashAttack
		EnhancedInputComponent->BindAction(SmashAttackAction, ETriggerEvent::Started, this, &AFHPlayerCharacter::SmashAttack);
		//Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AFHPlayerCharacter::Interaction);
		//Quick Slots
		EnhancedInputComponent->BindAction<AFHPlayerCharacter, int32>(QuickSlot1Action, ETriggerEvent::Started, this, &AFHPlayerCharacter::UseQuickSlot, 1);
		EnhancedInputComponent->BindAction<AFHPlayerCharacter, int32>(QuickSlot2Action, ETriggerEvent::Started, this, &AFHPlayerCharacter::UseQuickSlot, 2);
		EnhancedInputComponent->BindAction<AFHPlayerCharacter, int32>(QuickSlot3Action, ETriggerEvent::Started, this, &AFHPlayerCharacter::UseQuickSlot, 3);
		EnhancedInputComponent->BindAction<AFHPlayerCharacter, int32>(QuickSlot4Action, ETriggerEvent::Started, this, &AFHPlayerCharacter::UseQuickSlot, 4);
		EnhancedInputComponent->BindAction<AFHPlayerCharacter, int32>(QuickSlot5Action, ETriggerEvent::Started, this, &AFHPlayerCharacter::UseQuickSlot, 5);
		EnhancedInputComponent->BindAction<AFHPlayerCharacter, int32>(QuickSlot6Action, ETriggerEvent::Started, this, &AFHPlayerCharacter::UseQuickSlot, 6);
	}
}

bool AFHPlayerCharacter::CanPlayMontage()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (!AnimInst)
	{
		UE_LOG(LogTemp, Error, TEXT("Anim Instance is Null"));
		return false;
	}

	if (AnimInst->IsAnyMontagePlaying())
	{
		FString CurrentSectionName = AnimInst->Montage_GetCurrentSection().ToString();
		if (CurrentSectionName.Contains(TEXT("End")))
		{
			UE_LOG(LogTemp, Error, TEXT("End"));

			return true;
		}
		UE_LOG(LogTemp, Error, TEXT("MontagePlaying"));

		return false;
	}
	UE_LOG(LogTemp, Error, TEXT("Montage Not Playing"));

	return true;
}

void AFHPlayerCharacter::Jump()
{
	if (!CanPlayMontage() || GetCharacterMovement()->IsFalling())
	{
		return;
	}

	Super::Jump();
}

void AFHPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFHPlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to Controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFHPlayerCharacter::Dash(const FInputActionValue& Value)
{
	if (bIsDash)
	{
		return;
	}

	if (!CanPlayMontage() || GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (PlayerStatusComp->GetCurrentPlayerStmina() < DashStamina)
	{
		return;
	}

	PlayerStatusComp->UpdateCurrentPlayerStats(0, -DashStamina);

	bIsDash = true;
	FTimerHandle DashCoolTimeHandle;
	GetWorldTimerManager().SetTimer(DashCoolTimeHandle, [&]() { bIsDash = false; }, 1.5f, false);

	Req_Dash();
}

void AFHPlayerCharacter::Req_Dash_Implementation()
{
	Res_Dash();
}

void AFHPlayerCharacter::Res_Dash_Implementation()
{
	CHECK_VALID(DashMontage);
	PlayAnimMontage(DashMontage);
}

void AFHPlayerCharacter::NormalAttack(const FInputActionValue& Value)
{
	// Check WeaponMesh is not empty
	if (!Weapon->GetSkeletalMeshAsset())
	{
		return;
	}

	// check can attack
	if (!CanPlayMontage() || GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (PlayerStatusComp->GetCurrentPlayerStmina() < NormalAttackStamina)
	{
		return;
	}

	PlayerStatusComp->UpdateCurrentPlayerStats(0, -NormalAttackStamina);

	if (IWeaponInterface* WeaponInterfaceObj = Cast<IWeaponInterface>(Weapon))
	{
		WeaponInterfaceObj->Execute_EventNormalAttack(Weapon, this);
	}
}

void AFHPlayerCharacter::SmashAttack(const FInputActionValue& Value)
{
	// Check WeaponMesh is not empty
	if (!Weapon->GetSkeletalMeshAsset())
	{
		return;
	}

	// check can attack
	if (!CanPlayMontage() || GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (PlayerStatusComp->GetCurrentPlayerStmina() < SmashAttackStamina)
	{
		return;
	}

	PlayerStatusComp->UpdateCurrentPlayerStats(0, -SmashAttackStamina);

	if (IWeaponInterface* WeaponInterfaceObj = Cast<IWeaponInterface>(Weapon))
	{
		WeaponInterfaceObj->Execute_EventSmashAttack(Weapon, this);
	}
}

void AFHPlayerCharacter::Req_Attack_Implementation(class UAnimMontage* AttackMontage, FName SectionName)
{
	Res_Attack(AttackMontage, SectionName);
}

void AFHPlayerCharacter::Res_Attack_Implementation(class UAnimMontage* AttackMontage, FName SectionName)
{
	PlayAnimMontage(AttackMontage, 1.0f, SectionName);
}

void AFHPlayerCharacter::Interaction(const FInputActionValue& Value)
{
	if (!CanPlayMontage() || GetCharacterMovement()->IsFalling())
	{
		return;
	}

	// return if interacting is executing
	if (InteractingActor)
	{
		return;
	}

	TArray<AActor*> OverlappingActors;
	InteractCollision->GetOverlappingActors(OverlappingActors);

	// Find Nearest InterfaceObj
	double NearestLength = 9999.0f;

	for (AActor* Actor : OverlappingActors)
	{
		double distance = FVector::Dist(Actor->GetActorLocation(), GetActorLocation());
		if (NearestLength < distance)
		{
			continue;
		}

		// Check If Actor Inherits any Interfaces
		if (Cast<IInteractionInterface>(Actor))
		{
			NearestLength = distance;
			InteractingActor = Actor;
		}
	}

	if (IInteractionInterface* NearestItemInterfaceObj = Cast<IInteractionInterface>(InteractingActor))
	{
		NearestItemInterfaceObj->Execute_EventInteraction(InteractingActor, this);
	}
}

void AFHPlayerCharacter::Req_PickUp_Implementation(FRotator LookAtRot)
{
	Res_PickUp(LookAtRot);
}

void AFHPlayerCharacter::Res_PickUp_Implementation(FRotator LookAtRot)
{
	// Set Onwer Character Rotation to Look at this Item
	SetActorRotation(LookAtRot);

	// Play Interaction Montage
	CHECK_VALID(LootingMontage);
	PlayAnimMontage(LootingMontage);
}

void AFHPlayerCharacter::UseQuickSlot(int32 SlotNum)
{
	QuickSlotComp->UseQuickSlotItem(SlotNum - 1);
}

void AFHPlayerCharacter::OnWeaponUpdate(UItemData* UpdateEquipItem, const bool& bIsEquip)
{
	FWeaponItemData UpdateWeaponItemData;
	if (!UpdateEquipItem->GetWeaponData(UpdateWeaponItemData))
	{
		return;
	}

	Req_OnWeaponUpdate(UpdateWeaponItemData, bIsEquip);
}

void AFHPlayerCharacter::Req_OnWeaponUpdate_Implementation(const FWeaponItemData UpdateWeaponItemData, const bool bIsEquip)
{
	Res_OnWeaponUpdate(UpdateWeaponItemData, bIsEquip);
}

void AFHPlayerCharacter::Res_OnWeaponUpdate_Implementation(const FWeaponItemData UpdateWeaponItemData, const bool bIsEquip)
{
	Weapon->SetEquipMesh(UpdateWeaponItemData.WeaponMesh, bIsEquip, UpdateWeaponItemData.NormalAttackMontage, UpdateWeaponItemData.SmashAttackMontage);
}

void AFHPlayerCharacter::OnArmorUpdate(const EArmorType& UpdateArmorType, UItemData* UpdateEquipItem, const bool& bIsEquip)
{
	FArmorItemData UpdateArmorItemData;
	if (!UpdateEquipItem->GetArmorData(UpdateArmorItemData))
	{
		return;
	}

	Req_OnArmorUpdate(UpdateArmorType, UpdateArmorItemData, bIsEquip);
}

void AFHPlayerCharacter::Req_OnArmorUpdate_Implementation(const EArmorType UpdateArmorType, const FArmorItemData UpdateArmorItemData, const bool bIsEquip)
{
	Res_OnArmorUpdate(UpdateArmorType, UpdateArmorItemData, bIsEquip);
}

void AFHPlayerCharacter::Res_OnArmorUpdate_Implementation(const EArmorType UpdateArmorType, const FArmorItemData UpdateArmorItemData, const bool bIsEquip)
{
	for (auto ArmorMSMComp : ArmorMSMCompArray)
	{
		if (ArmorMSMComp->GetArmorType() != UpdateArmorType)
		{
			continue;
		}

		ArmorMSMComp->SetEquipMesh(UpdateArmorItemData.ArmorMesh, bIsEquip);

		// if Gloves
		if (UpdateArmorType == EArmorType::Gloves)
		{
			Glove_R->SetEquipMesh(UpdateArmorItemData.AdditionalArmorMesh, bIsEquip);
			return;
		}

		// if Upper - check bShouldSwitchBetweenAdditionalMesh, bIsEquip
		if (UpdateArmorType == EArmorType::Upper)
		{
			Cloak->SetEquipMesh(UpdateArmorItemData.AdditionalArmorMesh, bIsEquip);

			Cloak->SetVisibility(false);
		}

		// if Helmet
		Helmet->SetVisibility(true);
		if (UpdateArmorType == EArmorType::Helmet && UpdateArmorItemData.bHideHairWhenVisible)
		{
			Helmet->SetbHideHairWhenVisible(bIsEquip);
			Hair->SetVisibility(!bIsEquip);
		}
	}
}

void AFHPlayerCharacter::OnEquipVisibilityUpdate(const EArmorType& UpdateArmorType)
{
	Req_OnEquipVisibilityUpdate(UpdateArmorType);
}

void AFHPlayerCharacter::Req_OnEquipVisibilityUpdate_Implementation(const EArmorType UpdateArmorType)
{
	Res_OnEquipVisibilityUpdate(UpdateArmorType);
}

void AFHPlayerCharacter::Res_OnEquipVisibilityUpdate_Implementation(const EArmorType UpdateArmorType)
{
	switch (UpdateArmorType)
	{
	case EArmorType::Helmet:
		Helmet->ToggleVisibility();

		if (Helmet->GetbHideHairWhenVisible())
		{
			Hair->ToggleVisibility();
		}
		return;
	case EArmorType::Upper:
		Cloak->ToggleVisibility();
		return;
	default:
		break;
	}
}

void AFHPlayerCharacter::Req_UpdateCurrentHealth_Implementation(int32 DefaultHP, int32 CurHP)
{
	Res_UpdateCurrentHealth(DefaultHP, CurHP);
}

void AFHPlayerCharacter::Res_UpdateCurrentHealth_Implementation(int32 DefaultHP, int32 CurHP)
{
}
