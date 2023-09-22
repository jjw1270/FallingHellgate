// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusComponent.h"
#include "FallingHellgate.h"
#include "ItemData.h"
#include "Kismet/GameplayStatics.h"
#include "FHPlayerController.h"
#include "FHGameInstance.h"
#include "EquipmentComponent.h"
#include "FHPlayerCharacter.h"

// Sets default values for this component's properties
UPlayerStatusComponent::UPlayerStatusComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
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
	if (DefaultStatusUpdateDelegate.IsBound())
	{
		DefaultStatusUpdateDelegate.Broadcast(
			CurrentHealth,
			CurrentStamina,
			CurrentAttack,
			CurrentAttackSpeed,
			CurrentCritcal,
			CurrentDefence
		);
	}

	if (CurrentStatusUpdateDelegate.IsBound())
	{
		CurrentStatusUpdateDelegate.Broadcast(
			CurrentHealth,
			CurrentStamina,
			CurrentAttack,
			CurrentAttackSpeed,
			CurrentCritcal,
			CurrentDefence
		);
	}

	PrevStamina = CurrentStamina;
	bCanRegenStamina = true;
	GetWorld()->GetTimerManager().SetTimer(RegenStaminaHandle, this, &UPlayerStatusComponent::RegenStamina, 0.2f, true);
}

void UPlayerStatusComponent::RegenStamina()
{
	if (!bCanRegenStamina)
	{
		return;
	}

	if (PrevStamina > CurrentStamina)
	{
		bCanRegenStamina = false;

		FTimerHandle StaminaRegenWaitHandle;
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenWaitHandle, [&]() { bCanRegenStamina = true; }, 1.f, false);
		PrevStamina = CurrentStamina;
		
		return;
	}

	CurrentStamina += 1;

	PrevStamina = CurrentStamina;

	if (CurrentStamina > GI->GetDefaultPlayerStats().DefaultStamina)
	{
		CurrentStamina = GI->GetDefaultPlayerStats().DefaultStamina;
		return;
	}

	if (CurrentStatusUpdateDelegate.IsBound())
	{
		CurrentStatusUpdateDelegate.Broadcast(
			CurrentHealth,
			CurrentStamina,
			CurrentAttack,
			CurrentAttackSpeed,
			CurrentCritcal,
			CurrentDefence
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

		//UpdateCurrentPlayerStats(AddHealth, AddStamina);
	}
	else
	{
		GI->GetDefaultPlayerStats().DefaultHealth -= AddHealth;
		GI->GetDefaultPlayerStats().DefaultStamina -= AddStamina;
		GI->GetDefaultPlayerStats().DefaultAttack -= AddAttack;
		GI->GetDefaultPlayerStats().DefaultAttackSpeed -= AddAttackSpeed;
		GI->GetDefaultPlayerStats().DefaultCritcal -= AddCritcal;
		GI->GetDefaultPlayerStats().DefaultDefence -= AddDefence;

		//UpdateCurrentPlayerStats(-AddHealth, -AddStamina);
	}

	AFHPlayerCharacter* PlayerChar = GetOwner<AFHPlayerCharacter>();
	CHECK_VALID(PlayerChar);
	PlayerChar->Req_UpdateCurrentHealth(GI->GetDefaultPlayerStats().DefaultHealth, CurrentHealth);

	//BroadCast to stat UI
	if (DefaultStatusUpdateDelegate.IsBound())
	{
		DefaultStatusUpdateDelegate.Broadcast(
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
	if (GI->GetDefaultPlayerStats().DefaultHealth < CurrentHealth)
	{
		CurrentHealth = GI->GetDefaultPlayerStats().DefaultHealth;
	}
	else
	{
		CurrentHealth += AddHealth;
	}

	AFHPlayerCharacter* PlayerChar = GetOwner<AFHPlayerCharacter>();
	CHECK_VALID(PlayerChar);
	PlayerChar->Req_UpdateCurrentHealth(GI->GetDefaultPlayerStats().DefaultHealth, CurrentHealth);

	// if Updated default value is less then current value, current value = default vault
	if (GI->GetDefaultPlayerStats().DefaultStamina < CurrentStamina)
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

	//BroadCast to stat UI
	if (CurrentStatusUpdateDelegate.IsBound())
	{
		CurrentStatusUpdateDelegate.Broadcast(
			CurrentHealth,
			CurrentStamina,
			CurrentAttack,
			CurrentAttackSpeed,
			CurrentCritcal,
			CurrentDefence
		);
	}
}

const int32& UPlayerStatusComponent::GetCurrentPlayerHealth() const
{
	return CurrentHealth;
}

const int32& UPlayerStatusComponent::GetCurrentPlayerStmina() const
{
	return CurrentStamina;
}
