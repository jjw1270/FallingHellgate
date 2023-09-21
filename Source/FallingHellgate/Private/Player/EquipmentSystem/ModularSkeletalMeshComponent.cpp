// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularSkeletalMeshComponent.h"
#include "FallingHellgate.h"

UModularSkeletalMeshComponent::UModularSkeletalMeshComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UModularSkeletalMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	InitDefaultSkeletalMesh();
}

void UModularSkeletalMeshComponent::InitDefaultSkeletalMesh()
{
	DefaultSkeletalMeshAsset = GetSkeletalMeshAsset();
}

void UModularSkeletalMeshComponent::SetEquipMesh(USkeletalMesh* NewArmorMesh, const bool& bIsEquip)
{
	// if UnEquip, Set default Mesh
	if (!bIsEquip)
	{
		SetSkeletalMeshAsset(DefaultSkeletalMeshAsset);

		return;
	}

	CHECK_VALID(NewArmorMesh);
	SetSkeletalMeshAsset(NewArmorMesh);
}

