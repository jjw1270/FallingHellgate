// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "FHMCharacter.h"
#include "GameFramework/Character.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	WeaponMesh->SetCollisionProfileName("weapon");
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, OwnChar);
	DOREPLIFETIME(AWeapon, bIsMontagePlaying);
	DOREPLIFETIME(AWeapon, FHMCharactersToIgnore);
	DOREPLIFETIME(AWeapon, Range);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AFHMCharacter> It(GetWorld()); It; ++It)
	{
		FHMCharactersToIgnore.Add(*It);
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::EventAttack_Implementation()
{
}

void AWeapon::EventTrigger_Implementation()
{
	if (HasAuthority())
	{
		PlayAttackMontage();
	}
}

void AWeapon::EventPhase_Implementation()
{
	if (HasAuthority())
	{
		PlayPhaseChangeMontage();
	}
}

void AWeapon::PlayAttackMontage_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("AWeapon::PlayAttackMontage_Implementation called"));

	for (UAnimMontage* Anim : AttackAnimations)
	{
		if (!Anim)
		{
			UE_LOG(LogTemp, Warning, TEXT("AttackAnimations array Null"));
		}
	}

	if (AttackAnimations.Num() > 0 && bIsMontagePlaying == false)
	{
		bIsMontagePlaying = true;
		if (OwnChar == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("AWeapon::PlayAttackMontage_Implementation: OwnChar is null."));
			return;
		}
		int32 Index = FMath::RandRange(0, AttackAnimations.Num() - 1);
		PlayAnimation(Index);
	}
}

void AWeapon::PlayAnimation_Implementation(int32 Index)
{
	if (!IsValid(OwnChar))
	{
		UE_LOG(LogTemp, Error, TEXT("AWeapon::Multicast_PlayAnimation: OwnChar is null."));
		return;
	}

	UAnimInstance* AnimInstance = OwnChar->GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		APlayerController* AIController = Cast<APlayerController>(OwnChar->GetController());
		if (AIController)
		{
			AIController->SetIgnoreMoveInput(true);
		}
		
		OwnChar->PlayAnimMontage(AttackAnimations[Index]);

		if (HasAuthority())
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &AWeapon::OnMontageEnded);
		}
	}
}

void AWeapon::PlayPhaseChangeMontage_Implementation()
{

	if (bIsMontagePlaying == false)
	{
		bIsMontagePlaying = true;
		if (OwnChar == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("AWeapon::PlayAttackMontage_Implementation: OwnChar is null."));
			return;
		}
		PlayPhaseChangeAnimation();
	}
}

void AWeapon::PlayPhaseChangeAnimation_Implementation()
{
	if (!IsValid(OwnChar))
	{
		UE_LOG(LogTemp, Error, TEXT("AWeapon::Multicast_PlayAnimation: OwnChar is null."));
		return;
	}

	UAnimInstance* AnimInstance = OwnChar->GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		APlayerController* AIController = Cast<APlayerController>(OwnChar->GetController());
		if (AIController)
		{
			AIController->SetIgnoreMoveInput(true);
		}

		AFHMCharacter* FHMChar = Cast<AFHMCharacter>(OwnChar);
		if (!FHMChar)
		{
			UE_LOG(LogTemp, Error, TEXT("AWeapon::Multicast_PlayAnimation: Failed to cast OwnChar to AFHMCharacter."));
			return;
		}

		//Phase ¼³Á¤
		if (FHMChar->PhaseCheck == 2)
		{
			Damage = 300.f;
			OwnChar->PlayAnimMontage(PhaseChangeAnim1);
		}
		else if (FHMChar->PhaseCheck == 3)
		{
			Damage = 600.f;
			OwnChar->PlayAnimMontage(PhaseChangeAnim2);
		}
		else
		{
			return;
		}

		if (HasAuthority())
		{
			Range = 800.f;
			AnimInstance->OnMontageEnded.AddDynamic(this, &AWeapon::OnMontageEnded);
		}
	}
}


void AWeapon::OnMontageEnded_Implementation(UAnimMontage* Montage, bool bInterrupted)
{
	if (HasAuthority())
	{
		bIsMontagePlaying = false;

		if (OwnChar == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("AWeapon::OnMontageEnded: OwnChar is null after animation ended."));
			return;
		}

		APlayerController* AIController = Cast<APlayerController>(OwnChar->GetController());
		if (AIController)
		{
			AIController->SetIgnoreMoveInput(false);
		}

		UAnimInstance* AnimInstance = OwnChar->GetMesh()->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &AWeapon::OnMontageEnded);
		}
	}

	Range = 200;

}
