// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/RootItem.h"
#include "Components/SphereComponent.h"

// Sets default values
ARootItem::ARootItem()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	SphereCollision->SetSimulatePhysics(true);
	SphereCollision->SetNotifyRigidBodyCollision(true);
	SphereCollision->SetCollisionProfileName(FName("DropItem"));
	SphereCollision->OnComponentHit.AddDynamic(this, &ARootItem::OnHit);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(SphereCollision);
	ItemMesh->SetCollisionProfileName(FName("NoCollision"));
}

// Called when the game starts or when spawned
void ARootItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARootItem::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	SphereCollision->SetSimulatePhysics(false);
	SphereCollision->SetNotifyRigidBodyCollision(false);
}

void ARootItem::EventInteraction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Interaction"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
		FString::Printf(TEXT("Interaction ActorName = %s"), *this->GetName()));

	// Play Interaction Montage
	// In Montage AnimNotify, make logic...
}
