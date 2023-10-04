// Fill out your copyright notice in the Description page of Project Settings.


#include "FHPlayerCharacter.h"
#include "FallingHellgate.h"
#include "FHGameInstance.h"
#include "GameFramework/GameModeBase.h"
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

#include "Blueprint/UserWidget.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"

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
	if (!PC)
	{
		return;
	}

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

	if (GetPlayerStatusComp()->GetCurrentPlayerStats().Health <= 0)
	{
		return false;
	}

	if (AnimInst->IsAnyMontagePlaying())
	{
		FString CurrentSectionName = AnimInst->Montage_GetCurrentSection().ToString();

		if (CurrentSectionName.Contains(TEXT("End"), ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		{
			AnimInst->StopAllMontages(.1f);
			return true;
		}

		if (CurrentSectionName.Contains(TEXT("None")))
		{
			return true;
		}

		return false;
	}

	return true;
}

void AFHPlayerCharacter::Jump()
{
	if (!CanPlayMontage() || GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (PlayerStatusComp->GetCurrentPlayerStats().Stamina < JumpStamina)
	{
		return;
	}

	PlayerStatusComp->UpdateCurrentPlayerStats(0, -JumpStamina);

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
	if (bIsDashCool || !CanPlayMontage() || GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (PlayerStatusComp->GetCurrentPlayerStats().Stamina < DashStamina)
	{
		return;
	}

	PlayerStatusComp->UpdateCurrentPlayerStats(0, -DashStamina);

	bIsDashCool = true;
	FTimerHandle DashCoolTimeHandle;
	GetWorldTimerManager().SetTimer(DashCoolTimeHandle, [&]() { bIsDashCool = false; }, 1.5f, false);

	FVector CurrentAcceleration = GetCharacterMovement()->GetCurrentAcceleration();

	FRotator DashRot = GetActorForwardVector().Rotation();

	(CurrentAcceleration.IsNearlyZero()) ? DashRot : DashRot = CurrentAcceleration.Rotation();

	C2S_Dash(DashRot);
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

	// Find NeaS2Ct InterfaceObj
	double NeaS2CtLength = 9999.0f;

	for (AActor* Actor : OverlappingActors)
	{
		double distance = FVector::Dist(Actor->GetActorLocation(), GetActorLocation());
		if (NeaS2CtLength < distance)
		{
			continue;
		}

		// Check If Actor Inherits any Interfaces
		if (Cast<IInteractionInterface>(Actor))
		{
			NeaS2CtLength = distance;
			InteractingActor = Actor;
		}
	}

	if (IInteractionInterface* NeaS2CtItemInterfaceObj = Cast<IInteractionInterface>(InteractingActor))
	{
		NeaS2CtItemInterfaceObj->Execute_EventInteraction(InteractingActor, this);
	}
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

	if (PlayerStatusComp->GetCurrentPlayerStats().Stamina < NormalAttackStamina)
	{
		return;
	}

	PlayerStatusComp->UpdateCurrentPlayerStats(0, -NormalAttackStamina);

	FVector CurrentAcceleration = GetCharacterMovement()->GetCurrentAcceleration();

	FRotator AttackRot = FRotator(0, GetControlRotation().Yaw, 0);

	(CurrentAcceleration.IsNearlyZero()) ? AttackRot : AttackRot = CurrentAcceleration.Rotation();

	if (IWeaponInterface* WeaponInterfaceObj = Cast<IWeaponInterface>(Weapon))
	{
		WeaponInterfaceObj->Execute_EventNormalAttack(Weapon, this, AttackRot);
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

	if (PlayerStatusComp->GetCurrentPlayerStats().Stamina < SmashAttackStamina)
	{
		return;
	}

	PlayerStatusComp->UpdateCurrentPlayerStats(0, -SmashAttackStamina);

	FVector CurrentAcceleration = GetCharacterMovement()->GetCurrentAcceleration();

	FRotator AttackRot = FRotator(0, GetControlRotation().Yaw, 0);

	(CurrentAcceleration.IsNearlyZero()) ? AttackRot : AttackRot = CurrentAcceleration.Rotation();

	if (IWeaponInterface* WeaponInterfaceObj = Cast<IWeaponInterface>(Weapon))
	{
		WeaponInterfaceObj->Execute_EventSmashAttack(Weapon, this, AttackRot);
	}
}

void AFHPlayerCharacter::UseQuickSlot(int32 SlotNum)
{
	if (!CanPlayMontage())
	{
		return;
	}

	if (QuickSlotComp->IsQuickSlotEmpty(SlotNum - 1))
	{
		return;
	}

	C2S_UseItem(SlotNum - 1);
}

void AFHPlayerCharacter::C2S_Dash_Implementation(const FRotator& DashRot)
{
	S2M_Dash(DashRot);
}

void AFHPlayerCharacter::S2M_Dash_Implementation(const FRotator& DashRot)
{
	// Set Onwer Character Rotation to Look at this Item
	SetActorRotation(DashRot);

	CHECK_VALID(DashMontage);
	PlayAnimMontage(DashMontage);

	bDodge = true;
}

void AFHPlayerCharacter::C2S_PickUp_Implementation(const FRotator& LookAtRot)
{
	S2M_PickUp(LookAtRot);
}

void AFHPlayerCharacter::S2M_PickUp_Implementation(const FRotator& LookAtRot)
{
	// Set Onwer Character Rotation to Look at this Item
	SetActorRotation(LookAtRot);

	// Play Interaction Montage
	CHECK_VALID(LootingMontage);
	PlayAnimMontage(LootingMontage);
}

void AFHPlayerCharacter::C2S_Attack_Implementation(const FRotator& AttackRot, class UAnimMontage* AttackMontage, const FName& SectionName, const float& AttackSpeed)
{
	S2M_Attack(AttackRot, AttackMontage, SectionName, AttackSpeed);
}

void AFHPlayerCharacter::S2M_Attack_Implementation(const FRotator& AttackRot, class UAnimMontage* AttackMontage, const FName& SectionName, const float& AttackSpeed)
{
	// Set Onwer Character Rotation to Look at this Item
	SetActorRotation(AttackRot);

	PlayAnimMontage(AttackMontage, AttackSpeed, SectionName);
}

// Only Run on Server
void AFHPlayerCharacter::ApplyDamage(AActor* TargetActor, const float& DamageCoefficient)
{
	// apply damage to target
	float Damage = PlayerStatusComp->GetCurrentPlayerStats().Attack * DamageCoefficient;
	Damage = FMath::RandRange(Damage * 0.9f, Damage * 1.1f);
	(FMath::FRand() <= PlayerStatusComp->GetCurrentPlayerStats().Critcal) ? Damage *= 1.5f : Damage;

	UGameplayStatics::ApplyDamage(TargetActor, (int32)Damage, GetController(), this, NULL);

	S2M_ApplyDamage();
}

void AFHPlayerCharacter::S2M_ApplyDamage_Implementation()
{
	CHECK_VALID(ApplyDamageCameraShakeClass);
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ApplyDamageCameraShakeClass, GetActorLocation(), 0.f, 1000.f);

	if (ApplyDamageSounds.IsEmpty())
	{
		return;
	}
	int32 RandomIdx = FMath::RandRange(0, ApplyDamageSounds.Num() - 1);
	USoundBase* ApplyDamageSound = ApplyDamageSounds[RandomIdx];
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ApplyDamageSound, GetActorLocation());
}

// Only Run on Server
float AFHPlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bDodge)
	{
		return 0.f;
	}

	float NewDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("Damaged : -%f"), Damage);
	UE_LOG(LogTemp, Warning, TEXT("Causer : %s"), *DamageCauser->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Damaged : %s"), *this->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Prev Health : %d"), GetPlayerStatusComp()->GetCurrentPlayerStats().Health);

	FVector DirectionToDamageCauser = (DamageCauser->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	DirectionToDamageCauser.Z = 0.0f;
	float DamagedYawRot = DirectionToDamageCauser.Rotation().Yaw + 90.f;

	EHitDirection HitDirection = EHitDirection::Front;
	if (DamagedYawRot < 60.f || DamagedYawRot >= 300.f)
	{
		HitDirection = EHitDirection::Front;
	}
	else if (DamagedYawRot >= 60.f && DamagedYawRot < 120.f)
	{
		HitDirection = EHitDirection::Right;
	}
	else if (DamagedYawRot >= 120.f && DamagedYawRot < 240.f)
	{
		HitDirection = EHitDirection::Back;
	}
	else if (DamagedYawRot >= 240.f && DamagedYawRot < 300.f)
	{
		HitDirection = EHitDirection::Left;
	}

	if (GetPlayerStatusComp()->GetCurrentPlayerStats().Health - NewDamage <= 0)
	{
		C2S_Death(HitDirection);
	}
	else
	{
		bool bShouldKnockBack = NewDamage >= 0.3f * GetPlayerStatusComp()->GetDefaultPlayerStats().Health;

		C2S_TakeDamage(HitDirection, bShouldKnockBack);
	}

	GetPlayerStatusComp()->UpdateCurrentPlayerStats(-NewDamage, 0);

	return NewDamage;
}

void AFHPlayerCharacter::C2S_TakeDamage_Implementation(const EHitDirection& HitDir, const bool bKnockBack)
{
	UAnimMontage* HitReactMontage = nullptr;

	if (bKnockBack)
	{
		switch (HitDir)
		{
		case EHitDirection::Front:
			HitReactMontage = KnockBackFrontMontage;
			break;
		case EHitDirection::Left:
		case EHitDirection::Right:
		case EHitDirection::Back:
			HitReactMontage = KnockBackBackMontage;
			break;
		}
	}
	else
	{
		switch (HitDir)
		{
		case EHitDirection::Front:
			HitReactMontage = HitReactFrontMontage;
			break;
		case EHitDirection::Back:
			HitReactMontage = HitReactBackMontage;
			break;
		case EHitDirection::Left:
			HitReactMontage = HitReactLeftMontage;
			break;
		case EHitDirection::Right:
			HitReactMontage = HitReactRightMontage;
			break;
		}
	}

	S2M_TakeDamage(HitReactMontage);
}

void AFHPlayerCharacter::S2M_TakeDamage_Implementation(class UAnimMontage* HitReactMontage)
{
	GetMesh()->GetAnimInstance()->StopAllMontages(0.3f);

	CHECK_VALID(HitReactMontage);
	PlayAnimMontage(HitReactMontage);

	CHECK_VALID(BloodEffect);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodEffect, GetActorLocation());

	FRandomStream Stream(FMath::Rand());

	CHECK_VALID(BloodDecalClass);
	for (int32 i = 0; i < Stream.RandRange(1, 4); i++)
	{
		FVector DecalLoc = GetMesh()->GetComponentLocation();
		DecalLoc.X = Stream.FRandRange(DecalLoc.X - 60.f, DecalLoc.X + 60.f);
		DecalLoc.Y = Stream.FRandRange(DecalLoc.Y - 60.f, DecalLoc.Y + 60.f);

		ADecalActor* BloodDecal = GetWorld()->SpawnActor<ADecalActor>(BloodDecalClass, DecalLoc, FRotator(0.f, -90.f, 0.f));
		if (BloodDecal)
		{
			BloodDecal->GetDecal()->DecalSize = FVector(Stream.FRandRange(20.f, 70.f));
			BloodDecal->GetDecal()->SetFadeOut(8, 3, true);
		}

		Stream.GenerateNewSeed();
	}

	if (AFHPlayerController* PC = GetController<AFHPlayerController>())
	{
		PC->ShowBloodScreen(false);
	}
}

void AFHPlayerCharacter::C2S_Death_Implementation(const EHitDirection& HitDir)
{
	UAnimMontage* DeathMontage = nullptr;

	switch (HitDir)
	{
	case EHitDirection::Left:
	case EHitDirection::Right:
	case EHitDirection::Front:
		CHECK_VALID(DeathFrontMontage);
		DeathMontage = DeathFrontMontage;
		break;
	case EHitDirection::Back:
		CHECK_VALID(DeathBackMontage);
		DeathMontage = DeathBackMontage;
		break;
	}

	S2M_Death(DeathMontage);
}

void AFHPlayerCharacter::S2M_Death_Implementation(class UAnimMontage* DeathMontage)
{
	GetMesh()->GetAnimInstance()->StopAllMontages(0.3f);

	float MontageTime = PlayAnimMontage(DeathMontage);

	if (Tags.Contains(TEXT("Enemy")))
	{
		Tags.Remove(TEXT("Enemy"));
	}

	CHECK_VALID(BloodEffect);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodEffect, GetActorLocation());

	FRandomStream Stream(FMath::Rand());

	CHECK_VALID(BloodDecalClass);
	for (int32 i = 0; i < Stream.RandRange(1, 7); i++)
	{
		FVector DecalLoc = GetMesh()->GetComponentLocation();
		DecalLoc.X = Stream.FRandRange(DecalLoc.X - 80.f, DecalLoc.X + 80.f);
		DecalLoc.Y = Stream.FRandRange(DecalLoc.Y - 80.f, DecalLoc.Y + 80.f);

		ADecalActor* BloodDecal = GetWorld()->SpawnActor<ADecalActor>(BloodDecalClass, DecalLoc, FRotator(0.f, -90.f, 0.f));
		if (BloodDecal)
		{
			BloodDecal->GetDecal()->DecalSize = FVector(Stream.FRandRange(30.f, 80.f));
			BloodDecal->GetDecal()->SetFadeOut(10, 5, true);
		}

		Stream.GenerateNewSeed();
	}

	if (AFHPlayerController* PC = GetController<AFHPlayerController>())
	{
		PC->ShowBloodScreen(true);
	}

	MontageTime += 2.f;
	FTimerHandle DeathHandle;
	GetWorld()->GetTimerManager().SetTimer(DeathHandle, this, &AFHPlayerCharacter::Respawn, MontageTime, false);
}

void AFHPlayerCharacter::Respawn()
{
	GetMesh()->GetAnimInstance()->Montage_Stop(0.f);

	if (AGameModeBase* GM = GetWorld()->GetAuthGameMode())
	{
		GM->RestartPlayer(GetController());
		S2M_Respawn();
	}
}

void AFHPlayerCharacter::S2M_Respawn_Implementation()
{
	//SetActorLocation();
	GetPlayerStatusComp()->InitCurrentPlayerStats();
	Tags.Add("Enemy");
}

void AFHPlayerCharacter::C2S_UseItem_Implementation(const int32& QuickSlotIdx)
{
	S2M_UseItem();

	S2C_UseItem(QuickSlotIdx);
}

void AFHPlayerCharacter::S2M_UseItem_Implementation()
{
	CHECK_VALID(DrinkMontage);
	PlayAnimMontage(DrinkMontage);
}

void AFHPlayerCharacter::S2C_UseItem_Implementation(const int32& QuickSlotIdx)
{
	if (!GetController())
	{
		return;
	}

	UFHGameInstance* GI = GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	TempUseItem = *GI->GetQuickSlotItems()->Find(QuickSlotIdx);
}

void AFHPlayerCharacter::C2S_PlayUseItemEffect_Implementation(const EEffectTarget& EffectTarget)
{
	S2M_PlayUseItemEffect(EffectTarget);
}

void AFHPlayerCharacter::S2M_PlayUseItemEffect_Implementation(const EEffectTarget& EffectTarget)
{
	switch(EffectTarget)
	{
	case EEffectTarget::Health:
		CHECK_VALID(HealthParticleSystem);
		UGameplayStatics::SpawnEmitterAttached(HealthParticleSystem, GetMesh());
		break;
	case EEffectTarget::Stamina:
		CHECK_VALID(StaminaParticleSystem);
		UGameplayStatics::SpawnEmitterAttached(StaminaParticleSystem, GetMesh());
		break;
	case EEffectTarget::AttackPower:
		CHECK_VALID(AttackPowerParticleSystem);
		UGameplayStatics::SpawnEmitterAttached(AttackPowerParticleSystem, GetMesh());
		break;
	case EEffectTarget::DefensivePower:
		CHECK_VALID(DefensivePowerParticleSystem);
		UGameplayStatics::SpawnEmitterAttached(DefensivePowerParticleSystem, GetMesh());
		break;
	case EEffectTarget::AttackSpeed:
	case EEffectTarget::CriticalChance:
	default:
		break;
	}

	CHECK_VALID(ApplyItemSound);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ApplyItemSound, GetActorLocation());
}

void AFHPlayerCharacter::OnWeaponUpdate(UItemData* UpdateEquipItem, const bool& bIsEquip)
{
	FWeaponItemData UpdateWeaponItemData;
	if (!UpdateEquipItem->GetWeaponData(UpdateWeaponItemData))
	{
		return;
	}

	C2S_OnWeaponUpdate(UpdateWeaponItemData, bIsEquip);
}

void AFHPlayerCharacter::C2S_OnWeaponUpdate_Implementation(const FWeaponItemData& UpdateWeaponItemData, const bool bIsEquip)
{
	S2M_OnWeaponUpdate(UpdateWeaponItemData, bIsEquip);
}

void AFHPlayerCharacter::S2M_OnWeaponUpdate_Implementation(const FWeaponItemData& UpdateWeaponItemData, const bool bIsEquip)
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

	C2S_OnArmorUpdate(UpdateArmorType, UpdateArmorItemData, bIsEquip);
}

void AFHPlayerCharacter::C2S_OnArmorUpdate_Implementation(const EArmorType& UpdateArmorType, const FArmorItemData& UpdateArmorItemData, const bool bIsEquip)
{
	S2M_OnArmorUpdate(UpdateArmorType, UpdateArmorItemData, bIsEquip);
}

void AFHPlayerCharacter::S2M_OnArmorUpdate_Implementation(const EArmorType& UpdateArmorType, const FArmorItemData& UpdateArmorItemData, const bool bIsEquip)
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
	C2S_OnEquipVisibilityUpdate(UpdateArmorType);
}

void AFHPlayerCharacter::C2S_OnEquipVisibilityUpdate_Implementation(const EArmorType& UpdateArmorType)
{
	S2M_OnEquipVisibilityUpdate(UpdateArmorType);
}

void AFHPlayerCharacter::S2M_OnEquipVisibilityUpdate_Implementation(const EArmorType& UpdateArmorType)
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
