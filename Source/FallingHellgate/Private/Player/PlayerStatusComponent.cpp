// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusComponent.h"
#include "FallingHellgate.h"
#include "ItemData.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "FHPlayerController.h"
#include "FHGameInstance.h"
#include "EquipmentComponent.h"

// Sets default values for this component's properties
UPlayerStatusComponent::UPlayerStatusComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);
}

void UPlayerStatusComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerStatusComponent, CurrentHealth);
	DOREPLIFETIME(UPlayerStatusComponent, CurrentStamina);
	DOREPLIFETIME(UPlayerStatusComponent, CurrentAttack);
	DOREPLIFETIME(UPlayerStatusComponent, CurrentAttackSpeed);
	DOREPLIFETIME(UPlayerStatusComponent, CurrentCritcal);
	DOREPLIFETIME(UPlayerStatusComponent, CurrentDefence);
}

// Called when the game starts
void UPlayerStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// Bind Equipment Update Delegates
	// Only for Local client
	AFHPlayerController* PC = GetOwner()->GetInstigatorController<AFHPlayerController>();
	CHECK_VALID(PC);

	//if (PC != UGameplayStatics::GetPlayerController(GetWorld(), 0))
	//{
	//	return;
	//}

	GI = PC->GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	InitCurrentPlayerStats();

	UEquipmentComponent* EquipmentComp = PC->GetEquipmentComp();
	CHECK_VALID(EquipmentComp);

	EquipmentComp->WeaponUpdateDelegate.AddUObject(this, &UPlayerStatusComponent::OnWeaponUpdate);
	EquipmentComp->ArmorUpdateDelegate.AddUObject(this, &UPlayerStatusComponent::OnArmorUpdate);
}

void UPlayerStatusComponent::InitCurrentPlayerStats()
{
	UE_LOG(LogTemp, Warning, TEXT("InitCurrentPlayerStats"));

	CurrentHealth = GI->GetDefaultPlayerStats().DefaultHealth;
	CurrentStamina = GI->GetDefaultPlayerStats().DefaultStamina;
	CurrentAttack = GI->GetDefaultPlayerStats().DefaultAttack;
	CurrentAttackSpeed = GI->GetDefaultPlayerStats().DefaultAttackSpeed;
	CurrentCritcal = GI->GetDefaultPlayerStats().DefaultCritcal;
	CurrentDefence = GI->GetDefaultPlayerStats().DefaultDefence;

	//BroadCast to stat UI
	if (StatusUpdateDelegate.IsBound())
	{
		StatusUpdateDelegate.Broadcast(
			GI->GetDefaultPlayerStats().DefaultHealth,
			GI->GetDefaultPlayerStats().DefaultStamina,
			GI->GetDefaultPlayerStats().DefaultAttack,
			GI->GetDefaultPlayerStats().DefaultAttackSpeed,
			GI->GetDefaultPlayerStats().DefaultCritcal,
			GI->GetDefaultPlayerStats().DefaultDefence
		);
	}
}

void UPlayerStatusComponent::OnWeaponUpdate(UItemData* UpdateEquipItem, const bool& bIsEquip)
{
	FWeaponItemData UpdateWeaponItemData;
	if (!UpdateEquipItem->GetWeaponData(UpdateWeaponItemData))
	{
		return;
	}

	UpdateDefaultPlayerStats(bIsEquip, 0, 0,
		UpdateWeaponItemData.AttackPower, UpdateWeaponItemData.AttackSpeed, UpdateWeaponItemData.CriticalChance, 0);
}

void UPlayerStatusComponent::OnArmorUpdate(const EArmorType& UpdateArmorType, UItemData* UpdateEquipItem, const bool& bIsEquip)
{
	FArmorItemData UpdateArmorItemData;
	if (!UpdateEquipItem->GetArmorData(UpdateArmorItemData))
	{
		return;
	}

	UpdateDefaultPlayerStats(bIsEquip, UpdateArmorItemData.Health, UpdateArmorItemData.Stamina, UpdateArmorItemData.AttackPower,
		UpdateArmorItemData.AttackSpeed, UpdateArmorItemData.CriticalChance, UpdateArmorItemData.DefensivePower);
}

void UPlayerStatusComponent::UpdateDefaultPlayerStats(const bool& bIsEquip, const int32& AddHealth, const int32& AddStamina, const int32& AddAttack, const float& AddAttackSpeed, const float& AddCritcal, const int32& AddDefence)
{
	if (bIsEquip)
	{
		GI->GetDefaultPlayerStats().DefaultHealth += AddHealth;
		GI->GetDefaultPlayerStats().DefaultStamina += AddStamina;
		GI->GetDefaultPlayerStats().DefaultAttack += AddAttack;
		GI->GetDefaultPlayerStats().DefaultAttackSpeed += AddAttackSpeed;
		GI->GetDefaultPlayerStats().DefaultCritcal += AddCritcal;
		GI->GetDefaultPlayerStats().DefaultDefence += AddDefence;

		UpdateCurrentPlayerStats(AddHealth, AddStamina);
	}
	else
	{
		GI->GetDefaultPlayerStats().DefaultHealth -= AddHealth;
		GI->GetDefaultPlayerStats().DefaultStamina -= AddStamina;
		GI->GetDefaultPlayerStats().DefaultAttack -= AddAttack;
		GI->GetDefaultPlayerStats().DefaultAttackSpeed -= AddAttackSpeed;
		GI->GetDefaultPlayerStats().DefaultCritcal -= AddCritcal;
		GI->GetDefaultPlayerStats().DefaultDefence -= AddDefence;

		UpdateCurrentPlayerStats(-AddHealth, -AddStamina);
	}

	// Save DefaultPlayerStats Data in Instance

	//BroadCast to stat UI
	if (StatusUpdateDelegate.IsBound())
	{
		StatusUpdateDelegate.Broadcast(
			GI->GetDefaultPlayerStats().DefaultHealth,
			GI->GetDefaultPlayerStats().DefaultStamina,
			GI->GetDefaultPlayerStats().DefaultAttack,
			GI->GetDefaultPlayerStats().DefaultAttackSpeed,
			GI->GetDefaultPlayerStats().DefaultCritcal,
			GI->GetDefaultPlayerStats().DefaultDefence
		);
	}
}

void UPlayerStatusComponent::UpdateCurrentPlayerStats(const int32& AddHealth, const int32& AddStamina)
{
	// if Updated default value is less then current value, current value = default vault
	if (AddHealth < 0 && GI->GetDefaultPlayerStats().DefaultHealth < CurrentHealth)
	{
		CurrentHealth = GI->GetDefaultPlayerStats().DefaultHealth;
	}
	else
	{
		CurrentHealth += AddHealth;
	}

	// if Updated default value is less then current value, current value = default vault
	if (AddStamina < 0 && GI->GetDefaultPlayerStats().DefaultStamina < CurrentStamina)
	{
		CurrentStamina = GI->GetDefaultPlayerStats().DefaultStamina;
	}
	else
	{
		CurrentStamina += AddStamina;
	}

	CurrentAttack = GI->GetDefaultPlayerStats().DefaultAttack;
	CurrentAttackSpeed = GI->GetDefaultPlayerStats().DefaultAttackSpeed;
	CurrentCritcal = GI->GetDefaultPlayerStats().DefaultCritcal;
	CurrentDefence = GI->GetDefaultPlayerStats().DefaultDefence;
}

void UPlayerStatusComponent::OnRep_CurrentHealth()
{
	// broadcast for nametag
}
