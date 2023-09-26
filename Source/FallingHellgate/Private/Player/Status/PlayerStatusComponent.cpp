// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusComponent.h"
#include "FallingHellgate.h"
#include "ItemData.h"
#include "Kismet/GameplayStatics.h"
#include "FHPlayerController.h"
#include "EquipmentComponent.h"
#include "FHPlayerCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPlayerStatusComponent::UPlayerStatusComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicated(true);

}

// Called when the game starts
void UPlayerStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// Bind Equipment Update Delegates
	// Only for Local client
	AFHPlayerController* PC = GetOwner()->GetInstigatorController<AFHPlayerController>();
	CHECK_VALID(PC);

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
	DefaultPlayerStats = GI->GetDefaultPlayerStats();
	Req_UpdateDefaultPlayerStats(DefaultPlayerStats);

	CurrentPlayerStats = DefaultPlayerStats;
	Req_UpdateCurrentPlayerStats(CurrentPlayerStats);

	PrevStamina = CurrentPlayerStats.Stamina;
	bCanRegenStamina = true;
	GetWorld()->GetTimerManager().SetTimer(RegenStaminaHandle, this, &UPlayerStatusComponent::RegenStamina, 0.02f, true);
}

void UPlayerStatusComponent::RegenStamina()
{
	if (!bCanRegenStamina)
	{
		return;
	}

	if (PrevStamina > CurrentPlayerStats.Stamina)
	{
		bCanRegenStamina = false;

		FTimerHandle StaminaRegenWaitHandle;
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenWaitHandle, [&]() { bCanRegenStamina = true; }, 1.f, false);
		PrevStamina = CurrentPlayerStats.Stamina;
		
		return;
	}

	CurrentPlayerStats.Stamina += 1;

	PrevStamina = CurrentPlayerStats.Stamina;

	if (CurrentPlayerStats.Stamina > DefaultPlayerStats.Stamina)
	{
		CurrentPlayerStats.Stamina = DefaultPlayerStats.Stamina;
		return;
	}

	if (CurrentStatusUpdateDelegate.IsBound())
	{
		CurrentStatusUpdateDelegate.Broadcast(
			CurrentPlayerStats.Health,
			CurrentPlayerStats.Stamina,
			CurrentPlayerStats.Attack,
			CurrentPlayerStats.AttackSpeed,
			CurrentPlayerStats.Critcal,
			CurrentPlayerStats.Defence
		);
	}
}

void UPlayerStatusComponent::Req_UpdateDefaultPlayerStats_Implementation(FPlayerStats NewDefultPlayerStats)
{
	Res_UpdateDefaultPlayerStats(NewDefultPlayerStats);
}

void UPlayerStatusComponent::Res_UpdateDefaultPlayerStats_Implementation(FPlayerStats NewDefultPlayerStats)
{
	DefaultPlayerStats = NewDefultPlayerStats;

	//BroadCast to stat UI
	if (DefaultStatusUpdateDelegate.IsBound())
	{
		DefaultStatusUpdateDelegate.Broadcast(
			DefaultPlayerStats.Health,
			DefaultPlayerStats.Stamina,
			DefaultPlayerStats.Attack,
			DefaultPlayerStats.AttackSpeed,
			DefaultPlayerStats.Critcal,
			DefaultPlayerStats.Defence
		);
	}
}

void UPlayerStatusComponent::Req_UpdateCurrentPlayerStats_Implementation(FPlayerStats NewCurrentPlayerStats)
{
	Res_UpdateCurrentPlayerStats(NewCurrentPlayerStats);
}

void UPlayerStatusComponent::Res_UpdateCurrentPlayerStats_Implementation(FPlayerStats NewCurrentPlayerStats)
{
	CurrentPlayerStats = NewCurrentPlayerStats;

	//BroadCast to stat UI
	if (CurrentStatusUpdateDelegate.IsBound())
	{
		CurrentStatusUpdateDelegate.Broadcast(
			CurrentPlayerStats.Health,
			CurrentPlayerStats.Stamina,
			CurrentPlayerStats.Attack,
			CurrentPlayerStats.AttackSpeed,
			CurrentPlayerStats.Critcal,
			CurrentPlayerStats.Defence
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
		DefaultPlayerStats.Health += AddHealth;
		DefaultPlayerStats.Stamina += AddStamina;
		DefaultPlayerStats.Attack += AddAttack;
		DefaultPlayerStats.AttackSpeed += AddAttackSpeed;
		DefaultPlayerStats.Critcal += AddCritcal;
		DefaultPlayerStats.Defence += AddDefence;

		UpdateCurrentPlayerStats(0, 0, AddAttack, AddAttackSpeed, AddCritcal, AddDefence);
	}
	else
	{
		DefaultPlayerStats.Health -= AddHealth;
		DefaultPlayerStats.Stamina -= AddStamina;
		DefaultPlayerStats.Attack -= AddAttack;
		DefaultPlayerStats.AttackSpeed -= AddAttackSpeed;
		DefaultPlayerStats.Critcal -= AddCritcal;
		DefaultPlayerStats.Defence -= AddDefence;

		UpdateCurrentPlayerStats(0, 0, -AddAttack, -AddAttackSpeed, -AddCritcal, -AddDefence);
	}

	Req_UpdateDefaultPlayerStats(DefaultPlayerStats);
}

void UPlayerStatusComponent::UpdateCurrentPlayerStats(const int32& AddHealth, const int32& AddStamina, const int32& AddAttack, const float& AddAttackSpeed, const float& AddCritcal, const int32& AddDefence)
{
	AFHPlayerCharacter* PlayerChar = GetOwner<AFHPlayerCharacter>();
	CHECK_VALID(PlayerChar);

	// if Updated default value is less then current value, current value = default vault
	CurrentPlayerStats.Health += AddHealth;

	if (DefaultPlayerStats.Health < CurrentPlayerStats.Health)
	{
		CurrentPlayerStats.Health = DefaultPlayerStats.Health;
	}
	else if(CurrentPlayerStats.Health <= 0)
	{
		CurrentPlayerStats.Health = 0;
	}

	// UE_LOG(LogTemp, Warning, TEXT("%d / %d"), CurrentPlayerStats.Health, DefaultPlayerStats.Health);

	// if Updated default value is less then current value, current value = default vault
	CurrentPlayerStats.Stamina += AddStamina;
	if (DefaultPlayerStats.Stamina < CurrentPlayerStats.Stamina)
	{
		CurrentPlayerStats.Stamina = DefaultPlayerStats.Stamina;
	}
	else if(CurrentPlayerStats.Stamina <= 0)
	{
		CurrentPlayerStats.Stamina = 0;
	}

	CurrentPlayerStats.Attack += AddAttack;
	CurrentPlayerStats.AttackSpeed += AddAttackSpeed;
	CurrentPlayerStats.Critcal += AddCritcal;
	CurrentPlayerStats.Defence += AddDefence;

	Req_UpdateCurrentPlayerStats(CurrentPlayerStats);

}
