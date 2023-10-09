// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"
#include "FHMCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ABow::ABow()
{
	bReplicates = true;

    BowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
    BowMesh->SetCollisionProfileName("weapon");
    BowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetRootComponent(BowMesh);

    Damage = 50;
}

void ABow::EventAttack_Implementation()
{
	FVector Start = BowMesh->GetSocketLocation("StartPoint");
	FRotator Direction = BowMesh->GetSocketRotation("StartPoint");

	FVector ShotDirection = Direction.Vector();

	FVector End = Start + (ShotDirection * 1500);

	ReqAttack(Start, End);
}

void ABow::ReqAttack_Implementation(FVector vStart, FVector vEnd)
{
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

    TArray<AActor*> ActorsToIgnore = FHMCharactersToIgnore;
    ActorsToIgnore.Add(this);

    AFHMCharacter* Monster = Cast<AFHMCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AFHMCharacter::StaticClass()));
    if (Monster)
        ActorsToIgnore.Add(Monster);

    FHitResult OutHit;

    bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(
        GetWorld(),
        vStart,
        vEnd,
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::None,
        OutHit,
        true
    );

    if (bHit)
    {
        AActor* HitActor = OutHit.GetActor();
        if (HitActor && HitActor != this && HitActor->Tags.Contains("Player"))
        {
            if (OwnChar->GetController())
            {
                S2CHitSound();
                UGameplayStatics::ApplyDamage(HitActor, Damage, OwnChar->GetController(), this, nullptr);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Controller is null"));
            }
        }
    }
}
