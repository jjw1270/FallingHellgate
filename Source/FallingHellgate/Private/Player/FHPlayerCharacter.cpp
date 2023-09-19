// Fill out your copyright notice in the Description page of Project Settings.


#include "FHPlayerCharacter.h"
#include "FallingHellgate.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
//Enhanced Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InteractionInterface.h"
#include "FHPlayerController.h"
#include "QuickSlotComponent.h"
#include "EquipmentComponent.h"

#include "BaseWeapon.h"
#include "WeaponInterface.h"

#include "ItemData.h"
#include "ModularSkeletalMeshComponent.h"

#include "PlayerStatusComponent.h"

AFHPlayerCharacter::AFHPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UModularSkeletalMeshComponent>(ACharacter::MeshComponentName = TEXT("LowerBody")))
{
	PrimaryActorTick.bCanEverTick = false;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	// Set JumpZVelocity 700.f(Default) -> 350.f
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

	Weapon = CreateDefaultSubobject<UModularSkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(Glove_R, TEXT("WEAPON_R"));
	Weapon->SetArmorType(EArmorType::None);
}

// Network Setting
void AFHPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFHPlayerCharacter, PlayerRotation);
}

void AFHPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<AFHPlayerController>(GetController());
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

void AFHPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If Has Authority, Set PlayerRotation Uproperty(Replicated)
	if (HasAuthority() == true)
	{
		PlayerRotation = GetControlRotation();
	}
}

FRotator AFHPlayerCharacter::GetPlayerRotation()
{
	//Get Player Character index 0
	ACharacter* PlayerCharacter0 = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	//If index 0 Player = Self, Get Controller Roatation
	if (PlayerCharacter0 == this)
	{
		return GetControlRotation();
	}

	return PlayerRotation;
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
		//Roll
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &AFHPlayerCharacter::RollInput);
		//Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AFHPlayerCharacter::SprintInput);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AFHPlayerCharacter::StopSprintInput);
		//Attack - RightClick
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Triggered, this, &AFHPlayerCharacter::RightClickInput);
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Completed, this, &AFHPlayerCharacter::StopRightClickInput);
		//Attack - LeftClick
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Triggered, this, &AFHPlayerCharacter::LeftClickInput);
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AFHPlayerCharacter::StopLeftClickInput);
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
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFHPlayerCharacter::RollInput(const FInputActionValue& Value)
{	
	//Server
	Req_DoRollMove();
}

void AFHPlayerCharacter::Req_DoRollMove_Implementation()
{
	//Client
	Res_DoRollMove();
}

void AFHPlayerCharacter::Res_DoRollMove_Implementation()
{
	UE_LOG(LogClass, Warning, TEXT("DoRollMove - Start"));

	// Play Target AnimMontage When Target AnimMontage Is not Playing
	if (bIsMontagePlaying() == true)
	{
		UE_LOG(LogClass, Warning, TEXT("DoRollMove::IsMontagePlaying == true"));
		return;
	}

	// If Character Is Falling = return
	if (GetCharacterMovement()->IsFalling() == true)
	{
		UE_LOG(LogClass, Warning, TEXT("DoRollMove::IsFalling == true"));
		return;
	}

	// if Character Is Crouched = return
	/*if (bIsCrouched == true)
	{
		UE_LOG(LogClass, Warning, TEXT("DoRollMove::IsCrouched == true"));
		return;
	}*/

	// If StandToRollMontage Is Not Valid = return
	if (IsValid(StandToRollMontage) == false)
	{
		UE_LOG(LogClass, Warning, TEXT("DoRollMove::IsValid(StandToRollMontage) == false"));
		return;
	}

	// If RunToRollMontage Is Not Valid = return
	if (IsValid(RunToRollMontage) == false)
	{
		UE_LOG(LogClass, Warning, TEXT("DoRollMove::IsValid(RunToRollMontage) == false"));
		return;
	}

	// Check Max Speed And Play AnimMontage by Speed Value
	if (GetCharacterMovement()->GetMaxSpeed() <= 500.0f)
	{
		// Check Montage Is Playing
		//bIsMontagePlaying = GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();

		UE_LOG(LogClass, Warning, TEXT("DoRollMove::PlayAnimMontage - StandToRollMontage"));
		PlayAnimMontage(StandToRollMontage);
	}
	else if (GetCharacterMovement()->GetMaxSpeed() > 500.0f)
	{
		// Check Montage Is Playing
		//bIsMontagePlaying = GetMesh()->GetAnimInstance()->IsAnyMontagePlaying();

		UE_LOG(LogClass, Warning, TEXT("DoRollMove::PlayAnimMontage - RunToRollMontage"));
		PlayAnimMontage(RunToRollMontage);
	}

	UE_LOG(LogClass, Warning, TEXT("DoRollMove - End"));
}

void AFHPlayerCharacter::SprintInput(const FInputActionValue& Value)
{
	Req_SetMaxWalkSpeed(750.0f);
}

void AFHPlayerCharacter::StopSprintInput(const FInputActionValue& Value)
{
	Req_SetMaxWalkSpeed(500.0f);
}

void AFHPlayerCharacter::Req_SetMaxWalkSpeed_Implementation(float NewSpeed)
{
	//Sprint and StopSprint Action Use This Function
	//Default Value 500.f
	//Walk = 500.0f, Sprint 750.0f
	UE_LOG(LogClass, Warning, TEXT("SetMaxWalkSpeed"));

	// Set MaxWalkSpeed New Speed - Server
	Res_SetMaxWalkSpeed(NewSpeed);
}

void AFHPlayerCharacter::Res_SetMaxWalkSpeed_Implementation(float NewSpeed)
{
	// Set MaxWalkSpeed New Speed - Client
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void AFHPlayerCharacter::RightClickInput(const FInputActionValue& Value)
{	
	//Check Character has EquipWeapon
	if (Weapon->GetSkeletalMeshAsset() == nullptr)
	{
		UE_LOG(LogClass, Warning, TEXT("RightClickInput::EquipWeapon == nullptr"));

		return;
	}

	//Server
	//IsPressed is true
	RightClickAttack(true);
}

void AFHPlayerCharacter::StopRightClickInput(const FInputActionValue& Value)
{
	//Check Character has EquipWeapon
	if (Weapon->GetSkeletalMeshAsset() == nullptr)
	{
		UE_LOG(LogClass, Warning, TEXT("StopRightClickInput::EquipWeapon == nullptr"));

		return;
	}

	//Server
	//IsPressed is false
	RightClickAttack(false);
}

void AFHPlayerCharacter::Req_RightClickAttack_Implementation(bool IsPressed)
{
	//Client
	Res_RightClickAttack(IsPressed);
}

void AFHPlayerCharacter::Res_RightClickAttack_Implementation(bool IsPressed)
{
	UE_LOG(LogClass, Warning, TEXT("Res_RightClickAttack - Start"));

	// Cast WeaponInterface - EquipWeapon
	IWeaponInterface* WeaponInterfaceObj = Cast<IWeaponInterface>(EquipWeapon);

	// Cast WeaponInterface pointer is nullptr = return
	if (WeaponInterfaceObj == nullptr)
	{
		UE_LOG(LogClass, Warning, TEXT("Res_RightClickAttack::WeaponInterfaceObj == nullptr"));
		return;
	}

	WeaponInterfaceObj->Execute_Event_RightClickAttack(EquipWeapon, IsPressed);

	UE_LOG(LogClass, Warning, TEXT("Res_RightClickAttack - End"));
}

void AFHPlayerCharacter::RightClickAttack(bool IsPressed)
{
	UE_LOG(LogClass, Warning, TEXT("RightClickAttack - Start"));

	// Cast WeaponInterface - EquipWeapon
	IWeaponInterface* WeaponInterfaceObj = Cast<IWeaponInterface>(EquipWeapon);

	// Cast WeaponInterface pointer is nullptr = return
	if (WeaponInterfaceObj == nullptr)
	{
		UE_LOG(LogClass, Warning, TEXT("RightClickAttack::WeaponInterfaceObj == nullptr"));
		return;
	}

	WeaponInterfaceObj->Execute_Event_RightClickAttack(EquipWeapon, IsPressed);

	UE_LOG(LogClass, Warning, TEXT("RightClickAttack - End"));
}


void AFHPlayerCharacter::LeftClickInput(const FInputActionValue& Value)
{
	//Check Character has EquipWeapon
	if (Weapon->GetSkeletalMeshAsset() == nullptr)
	{
		UE_LOG(LogClass, Warning, TEXT("LeftClickInput::EquipWeapon == nullptr"));

		return;
	}

	//Server
	//IsPressed is true
	LeftClickAttack(true);
}

void AFHPlayerCharacter::StopLeftClickInput(const FInputActionValue& Value)
{
	//Check Character has EquipWeapon
	if (Weapon->GetSkeletalMeshAsset() == nullptr)
	{
		UE_LOG(LogClass, Warning, TEXT("StopLeftClickInput::EquipWeapon == nullptr"));

		return;
	}

	//UE_LOG(LogClass, Warning, TEXT("LeftClickCount :: %d"), LeftClickCount);

	//Server
	//IsPressed is false
	LeftClickAttack(false);
}

void AFHPlayerCharacter::Req_LeftClickAttack_Implementation(bool IsPressed)
{
	//Client
	Res_LeftClickAttack(IsPressed);
}

void AFHPlayerCharacter::Res_LeftClickAttack_Implementation(bool IsPressed)
{
	UE_LOG(LogClass, Warning, TEXT("Res_LeftClickAttack - Start"));

	// Cast WeaponInterface - EquipWeapon
	IWeaponInterface* WeaponInterfaceObj = Cast<IWeaponInterface>(EquipWeapon);

	// Cast WeaponInterface pointer is nullptr = return
	if (WeaponInterfaceObj == nullptr)
	{
		UE_LOG(LogClass, Warning, TEXT("Res_LeftClickAttack::WeaponInterfaceObj == nullptr"));
		return;
	}

	WeaponInterfaceObj->Execute_Event_LeftClickAttack(EquipWeapon, IsPressed);

	UE_LOG(LogClass, Warning, TEXT("Res_LeftClickAttack - End"));
}

void AFHPlayerCharacter::LeftClickAttack(bool IsPressed)
{
	UE_LOG(LogClass, Warning, TEXT("LeftClickAttack - Start"));

	// Cast WeaponInterface - EquipWeapon
	IWeaponInterface* WeaponInterfaceObj = Cast<IWeaponInterface>(EquipWeapon);

	// Cast WeaponInterface pointer is nullptr = return
	if (WeaponInterfaceObj == nullptr)
	{
		UE_LOG(LogClass, Warning, TEXT("LeftClickAttack::WeaponInterfaceObj == nullptr"));
		return;
	}

	WeaponInterfaceObj->Execute_Event_LeftClickAttack(EquipWeapon, IsPressed);

	UE_LOG(LogClass, Warning, TEXT("LeftClickAttack - End"));
}


void AFHPlayerCharacter::Interaction(const FInputActionValue& Value)
{
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
	Weapon->SetEquipMesh(UpdateWeaponItemData.WeaponMesh, bIsEquip);
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
