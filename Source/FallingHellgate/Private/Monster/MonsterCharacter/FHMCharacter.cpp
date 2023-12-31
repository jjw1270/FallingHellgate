#include "FHMCharacter.h"
#include "FallingHellgate.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "FHMonsterStateComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTServiceLookPlayer.h"
#include "FHMonsterAIController.h"
#include "AIController.h"
#include "Weapon.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIDataTable.h"
#include "Net/UnrealNetwork.h"
#include "Spear.h"
#include "Sword.h"
#include "Bow.h"
#include "GreatSword.h"
#include "Components/DecalComponent.h"
#include "Engine/DecalActor.h"
#include "Blueprint/UserWidget.h"
#include "DropItem.h"

// Sets default values
AFHMCharacter::AFHMCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	//Movement State
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	AIControllerClass = AFHMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	//Ragdoll
	GetMesh()->SetCollisionProfileName("Ragdoll");
	IsRagdoll = false;
	bHasDown = false;

	MonsterStateComponent = CreateDefaultSubobject<UFHMonsterStateComponent>(TEXT("FHMonsterState"));

	//Enermy Tag
	Tags.Add(FName("Enemy"));
}

void AFHMCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFHMCharacter, OwnChar);
	DOREPLIFETIME(AFHMCharacter, EquippedWeapon);
	DOREPLIFETIME(AFHMCharacter, bIsBossCheck);
	DOREPLIFETIME(AFHMCharacter, PhaseCheck);
	DOREPLIFETIME(AFHMCharacter, WeaponTypes);
	DOREPLIFETIME(AFHMCharacter, bIsGroggy);
	DOREPLIFETIME(AFHMCharacter, IsRagdoll);
}


void AFHMCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetReplicates(true);
	SetReplicateMovement(true);

	ReqEquipWeapon(WeaponTypes);

	BindPlayerState();

	UFHMonsterStateComponent* StateComponent = Cast<UFHMonsterStateComponent>(GetComponentByClass(UFHMonsterStateComponent::StaticClass()));
	if (IsValid(StateComponent))
	{
		StateComponent->OwnerCharacter = this;

		if (bIsBossCheck)
		{
			StateComponent->bIsBoss = true;
		}
	}
}
// Called every frame
void AFHMCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFHMCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFHMCharacter::AttackTrigger_Implementation()
{
	ReqTrigger();
}

void AFHMCharacter::ReqTrigger_Implementation()
{
	ResTrigger();
}

void AFHMCharacter::ResTrigger_Implementation()
{
	IMonsterWeaponInterface* InterfaceObj = Cast<IMonsterWeaponInterface>(Weapon);

	if (InterfaceObj == nullptr)
		return;

	InterfaceObj->Execute_EventTrigger(Weapon);
}

void AFHMCharacter::OnUpdateHp_Implementation(float CurHp, float MaxHp)
{
}

void AFHMCharacter::BindPlayerState()
{
	UFHMonsterStateComponent* StateComponent = Cast<UFHMonsterStateComponent>(GetComponentByClass(UFHMonsterStateComponent::StaticClass()));
	if (IsValid(StateComponent))
	{
		StateComponent->Fuc_Dele_DynamicUpdateHp.AddDynamic(this, &AFHMCharacter::OnUpdateHp);
		OnUpdateHp(StateComponent->CurHp, StateComponent->MaxHp);
	}

	FTimerManager& timerManager = GetWorld()->GetTimerManager();
	timerManager.SetTimer(th_BindPlayerState, this, &AFHMCharacter::BindPlayerState, 0.1f, false);
}

void AFHMCharacter::S2CTakeBlood_Implementation()
{
	S2MTakeBlood();
}

void AFHMCharacter::S2MTakeBlood_Implementation()
{
	if (!BloodEffect)
		return ;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodEffect, GetActorLocation());

	FRandomStream Stream(FMath::Rand());

	if (!BloodDecalClass)
		return ;

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
}

void AFHMCharacter::S2CDoRagdoll_Implementation()
{
	DoRagdoll();
}

void AFHMCharacter::DoRagdoll_Implementation()
{
	if (IsRagdoll)
		return;

	IsRagdoll = true;

	GetMesh()->SetSimulatePhysics(true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
		EquippedWeapon = nullptr;
	}

	GetWorld()->GetTimerManager().SetTimer(th_DestroyActor, this, &AFHMCharacter::DestroyActor, 10.0f, false);
}

void AFHMCharacter::DestroyActor_Implementation()
{
	Destroy();
}

void AFHMCharacter::ReqEquipWeapon_Implementation(TSubclassOf<class AWeapon> WeaponClass)
{

	if (HasAuthority())
	{
		Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass, FVector(0, 0, 0), FRotator(0, 0, 0));

		EquippedWeapon = Cast<AWeapon>(Weapon);
		if (IsValid(EquippedWeapon) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("AFHMCharacter::EquipWeapon: Cast fail."));
			return;
		}

		OnRep_Equip();
	}
}

void AFHMCharacter::OnRep_Equip()
{

		if (IsValid(EquippedWeapon))
		{
			FName WeaponSocketName;

			AAIController* AIController = Cast<AAIController>(GetController());

			UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();

			UE_LOG(LogTemp, Warning, TEXT("AFHmcharacter :: Equip Weapon :  Equipped weapon is set"));
				
			if (BlackboardComp)
				{
					if (EquippedWeapon->IsA(ASpear::StaticClass()))
					{
						UE_LOG(LogTemp, Warning, TEXT("AFHMCharacter::EquipWeapon: Spear equipped."));
						BlackboardComp->SetValueAsClass("WeaponType", ASpear::StaticClass());
						WeaponSocketName = FName("WeaponSpear");
					}
					else if (EquippedWeapon->IsA(ASword::StaticClass()))
					{
						UE_LOG(LogTemp, Warning, TEXT("AFHMCharacter::EquipWeapon: Sword equipped."));
						BlackboardComp->SetValueAsClass("WeaponType", ASword::StaticClass());
						WeaponSocketName = FName("WeaponSword");
					}
					else if (EquippedWeapon->IsA(ABow::StaticClass()))
					{
						UE_LOG(LogTemp, Warning, TEXT("AFHMCharacter::Equip Weapon: Bow equipped."));
						BlackboardComp->SetValueAsClass("WeaponType", ABow::StaticClass());
						WeaponSocketName = FName("WeaponBow");
					}
					else if (EquippedWeapon->IsA(AGreatSword::StaticClass()))
					{
						UE_LOG(LogTemp, Warning, TEXT("AFHMCharacter::Equip Weapon: GreatSword equipped."));
						BlackboardComp->SetValueAsClass("WeaponType", AGreatSword::StaticClass());
						WeaponSocketName = FName("WeaponGreatSword");
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("AFHmcharacter :: Equip Weapont : Unknown or no weapont equipped."));
						WeaponSocketName = FName("Defaultsocket");
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("AFHmcharacter :: Equip Weapon : Black board comp is null"));
				}

				Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);

				EquippedWeapon->SetOwner(this);
				EquippedWeapon->OwnChar = this;
		}

}

void AFHMCharacter::S2CTakeGroggy_Implementation()
{
	TakeGroggy();
}

void AFHMCharacter::S2COnGroggyEnded_Implementation(UAnimMontage* Montage, bool bInterrupted)
{
	OnGroggyEnded(Montage, bInterrupted);
}

void AFHMCharacter::TakeGroggy_Implementation()
{
	bIsGroggy = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (AnimInstance && Hitstun)
	{
		if (MovementComponent)
		{
			MovementComponent->Deactivate();
		}

		AnimInstance->Montage_Play(Hitstun);
		AnimInstance->OnMontageEnded.AddDynamic(this, &AFHMCharacter::S2COnGroggyEnded);
	}
}

void AFHMCharacter::OnGroggyEnded_Implementation(UAnimMontage* Montage, bool bInterrupted)
{
	bIsGroggy = false;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (MovementComponent)
	{
		MovementComponent->Activate(true);
	}
}

float AFHMCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Enermy Take %f"), DamageAmount);

	FString CauserName = DamageCauser->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Damage caused by: %s"), *CauserName);

	UFHMonsterStateComponent* StateComponent = Cast<UFHMonsterStateComponent>(GetComponentByClass(UFHMonsterStateComponent::StaticClass()));
	if (IsValid(StateComponent))
	{
		StateComponent->AddDamage(DamageAmount);
	}

	APlayerController* PC = Cast<APlayerController>(EventInstigator);
	if (PC)
	{
		S2CTakeBlood();

		if (StateComponent->CurHp <= 0)
		{
			S2CDropItem();
		}
	}

	return DamageAmount;
}

void AFHMCharacter::S2CDropItem_Implementation()
{
	if (DropItemClass)
	{
		GetWorld()->SpawnActor<ADropItem>(DropItemClass, GetActorLocation(), FRotator());
	}
}

void AFHMCharacter::PhaseChangeTrigger_Implementation()
{
	ReqPhaseChange();
}


void AFHMCharacter::ReqPhaseChange_Implementation()
{
	ResPhaseChange();
}

void AFHMCharacter::ResPhaseChange_Implementation()
{
	IMonsterWeaponInterface* InterfaceObj = Cast<IMonsterWeaponInterface>(Weapon);

	if (InterfaceObj == nullptr)
		return;

	InterfaceObj->Execute_EventPhase(Weapon);
}

void AFHMCharacter::S2MStageClear_Implementation()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PlayerController != nullptr)
		{
			UUserWidget* SeenStageClearWidget = CreateWidget<UUserWidget>(PlayerController, StageClearWidgetClass);

			if (SeenStageClearWidget != nullptr)
			{
				SeenStageClearWidget->AddToViewport();

			}
		}
}

void AFHMCharacter::S2CStageClear_Implementation()
{
	S2MStageClear();
}
