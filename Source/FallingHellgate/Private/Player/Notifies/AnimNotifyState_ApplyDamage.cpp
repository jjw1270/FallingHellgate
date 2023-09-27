// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_ApplyDamage.h"
#include "FallingHellgate.h"
#include "FHPlayerCharacter.h"
#include "BaseWeapon.h"
#include "PlayerStatusComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "FHPlayerController.h"

UAnimNotifyState_ApplyDamage::UAnimNotifyState_ApplyDamage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//Do not Active Notify State in UE Editor(In Animation Montage Window)
	//If Do not this, UE Editor Maybe crashed
	bShouldFireInEditor = false;

	CollisionAttachSocket1 = TEXT("CollisionStart");
	CollisionAttachSocket2 = TEXT("CollisionEnd");
}

void UAnimNotifyState_ApplyDamage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	OwnCharacter = Cast<AFHPlayerCharacter>(MeshComp->GetOwner());
	CHECK_VALID(OwnCharacter);
	
	WeaponMesh = OwnCharacter->GetWeapon();
	CHECK_VALID(WeaponMesh);
}

void UAnimNotifyState_ApplyDamage::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!OwnCharacter->HasAuthority())
	{
		return;
	}

	TArray<FHitResult> OutResults;
	if (DetectWeaponCollsion(OutResults))
	{
		AddDamageToTargets(OutResults);
	}
}

void UAnimNotifyState_ApplyDamage::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	// AlreadyDamagedTargets.Empty();
}

bool UAnimNotifyState_ApplyDamage::DetectWeaponCollsion(TArray<FHitResult>& OutResults)
{
	FVector TraceStartPos;
	FVector TraceEndPos;

	TraceStartPos = WeaponMesh->GetSocketLocation(CollisionAttachSocket1);
	TraceEndPos = WeaponMesh->GetSocketLocation(CollisionAttachSocket2);

	FCollisionShape MySphere = FCollisionShape::MakeSphere(TraceSphereRadius);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnCharacter);
	bool IsHit = UKismetSystemLibrary::SphereTraceMultiForObjects(OwnCharacter->GetWorld(), TraceStartPos, TraceEndPos, TraceSphereRadius,
		ObjTypes, false, ActorsToIgnore, DrawDebugCollision.GetValue(), OutResults, false, FColor::Red, FColor::Green, 1.f);

	return IsHit;
}

void UAnimNotifyState_ApplyDamage::AddDamageToTargets(TArray<FHitResult> Results)
{
	for (const FHitResult& out : Results)
	{
		AActor* TargetActor = out.GetActor();
		if (!TargetActor)
		{
			continue;
		}

		bool bIsEnemy = TargetActor->ActorHasTag(TEXT("Enemy"));

		if (!bIsEnemy || AlreadyDamagedTargets.Contains(TargetActor))
		{
			continue;
		}

		AlreadyDamagedTargets.Add(TargetActor);

		UE_LOG(LogTemp, Warning, TEXT("ApplyDamage"));

		OwnCharacter->ApplyDamage(TargetActor, DamageCoefficient);
	}
}
