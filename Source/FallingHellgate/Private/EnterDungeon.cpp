// Fill out your copyright notice in the Description page of Project Settings.


#include "EnterDungeon.h"
#include "FallingHellgate.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FHPlayerController.h"

// Sets default values
AEnterDungeon::AEnterDungeon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	EnterDungeonCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(EnterDungeonCollision);

	EnterDungeonCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnterDungeon::OnCollisionEnter);

	EnterDungeonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	EnterDungeonMesh->SetupAttachment(EnterDungeonCollision);
}

// Called when the game starts or when spawned
void AEnterDungeon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnterDungeon::OnCollisionEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->GetInstigatorController())
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(OtherActor->GetInstigatorController());
	CHECK_VALID(PC);

	if (PC->HasAuthority())
	{
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
		{
			AFHPlayerController* PCOnServer = Cast<AFHPlayerController>(*Iter);
			if (PCOnServer)
			{
				PCOnServer->S2C_CreateEnterDungeonWidget(DungeonLevelName);
			}
		}

		GetWorld()->ServerTravel(DungeonLevelName);
	}
}
