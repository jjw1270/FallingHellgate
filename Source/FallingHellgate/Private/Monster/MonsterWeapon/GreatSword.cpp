// Fill out your copyright notice in the Description page of Project Settings.

#include "GreatSword.h"
#include "FHMCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AGreatSword::AGreatSword()
{
	bReplicates = true;

	SetRootComponent(WeaponMesh);

	Damage = 150.f;
}

void AGreatSword::EventAttack_Implementation()
{
	FVector Start = WeaponMesh->GetSocketLocation("StartPoint");
	FVector End = WeaponMesh->GetSocketLocation("EndPoint");

	ReqAttack(Start, End);
}

void AGreatSword::ReqAttack_Implementation(FVector vStart, FVector vEnd)
{
    //Hit Ã³¸®
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    AFHMCharacter* Monster = Cast<AFHMCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AFHMCharacter::StaticClass()));
    if (Monster)
        ActorsToIgnore.Add(Monster);

    TArray<FHitResult> OutHits;

    bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
        GetWorld(),
        vStart,
        vEnd,
        Range,
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::None,
        OutHits,
        true
    );

    if (bHit)
    {
        TSet<AActor*> HitActors;

        for (auto& Hit : OutHits)
        {
            AActor* HitActor = Hit.GetActor();
            if (HitActor && HitActor != this && !HitActors.Contains(HitActor))
            {
                if (OwnChar->GetController())
                {
                    S2CHitSound();
                    UGameplayStatics::ApplyDamage(HitActor, Damage, OwnChar->GetController(), this, nullptr);
                    HitActors.Add(HitActor);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Controller is null"));
                }
            }
        }
    }
}
