// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusComponent.h"
#include "FallingHellgate.h"
#include "ItemData.h"
#include "Kismet/GameplayStatics.h"
#include "FHPlayerController.h"
#include "EquipmentComponent.h"
#include "FHPlayerCharacter.h"
#include "Net/UnrealNetwork.h"
#include "FHHUD.h"
#include "HUDWidget.h"
#include "PartyInfoWidget.h"

UPlayerStatusComponent::UPlayerStatusComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	//SetIsReplicated(true);

	SetIsReplicatedByDefault(true);
}

void UPlayerStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// Bind Equipment Update Delegates
	// Only for Local client
	AFHPlayerController* PC = GetOwner()->GetInstigatorController<AFHPlayerController>();
	CHECK_VALID(PC);

	GI = PC->GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	UEquipmentComponent* EquipmentComp = PC->GetEquipmentComp();
	CHECK_VALID(EquipmentComp);

	EquipmentComp->WeaponUpdateDelegate.AddUObject(this, &UPlayerStatusComponent::OnWeaponUpdate);
	EquipmentComp->ArmorUpdateDelegate.AddUObject(this, &UPlayerStatusComponent::OnArmorUpdate);

	FTimerHandle InitCurrentPlayerStatsHandle;
	GetWorld()->GetTimerManager().SetTimer(InitCurrentPlayerStatsHandle, [&]() {InitCurrentPlayerStats(); }, 1.f, false);
}

void UPlayerStatusComponent::InitCurrentPlayerStats()
{
	CHECK_VALID(GI);
	PlayerName = GI->GetPlayerName();
	C2S_UpdatePlayerName(PlayerName);

	DefaultPlayerStats = GI->GetDefaultPlayerStats();
	C2S_UpdateDefaultPlayerStats(DefaultPlayerStats);

	CurrentPlayerStats = DefaultPlayerStats;
	C2S_UpdateCurrentPlayerStats(CurrentPlayerStats);

	PrevStamina = CurrentPlayerStats.Stamina;
	bCanRegenStamina = true;
	GetWorld()->GetTimerManager().SetTimer(RegenStaminaHandle, this, &UPlayerStatusComponent::RegenStamina, 0.05f, true);

	PrevHealth = CurrentPlayerStats.Health;
	bCanRegenHealth = true;
	GetWorld()->GetTimerManager().SetTimer(RegenHealthHandle, this, &UPlayerStatusComponent::RegenHealth, .5f, true);
}

void UPlayerStatusComponent::UpdateDefaultStatusToGameInst()
{
	CHECK_VALID(GI);
	GI->GetDefaultPlayerStats() = DefaultPlayerStats;
}

void UPlayerStatusComponent::UpdateDefaultStatusUI()
{
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

	UpdateDefaultStatusToGameInst();
}

void UPlayerStatusComponent::UpdateCurrentStatusUI()
{
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
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenWaitHandle, [&]() { bCanRegenStamina = true; }, 1.5f, false);
		PrevStamina = CurrentPlayerStats.Stamina;
		
		return;
	}

	if (CurrentPlayerStats.Stamina == DefaultPlayerStats.Stamina)
	{
		return;
	}

	UpdateCurrentPlayerStats(0, 1);

	PrevStamina = CurrentPlayerStats.Stamina;
}

void UPlayerStatusComponent::RegenHealth()
{
	if (!bCanRegenHealth)
	{
		return;
	}

	// On Death
	if (CurrentPlayerStats.Health <= 0)
	{
		bCanRegenHealth = false;
		GetWorld()->GetTimerManager().ClearTimer(RegenStaminaHandle);
		GetWorld()->GetTimerManager().ClearTimer(RegenHealthHandle);
		return;
	}

	if (PrevHealth > CurrentPlayerStats.Health)
	{
		bCanRegenHealth = false;

		FTimerHandle HealthRegenWaitHandle;
		GetWorld()->GetTimerManager().SetTimer(HealthRegenWaitHandle, [&]() { bCanRegenHealth = true; }, 4.f, false);
		PrevHealth = CurrentPlayerStats.Health;

		return;
	}

	if (CurrentPlayerStats.Health == DefaultPlayerStats.Health)
	{
		return;
	}

	UpdateCurrentPlayerStats(1, 0);

	PrevHealth = CurrentPlayerStats.Health;
}

void UPlayerStatusComponent::C2S_UpdatePlayerName_Implementation(const FText& NewPlayerName)
{
	S2M_UpdatePlayerName(NewPlayerName);
}

void UPlayerStatusComponent::S2M_UpdatePlayerName_Implementation(const FText& NewPlayerName)
{
	PlayerName = NewPlayerName;
}

void UPlayerStatusComponent::C2S_UpdateDefaultPlayerStats_Implementation(const FPlayerStats& NewDefultPlayerStats)
{
	S2M_UpdateDefaultPlayerStats(NewDefultPlayerStats);
}

void UPlayerStatusComponent::S2M_UpdateDefaultPlayerStats_Implementation(const FPlayerStats& NewDefultPlayerStats)
{
	DefaultPlayerStats = NewDefultPlayerStats;
	UpdateDefaultStatusUI();
}

void UPlayerStatusComponent::C2S_UpdateCurrentPlayerStats_Implementation(const FPlayerStats& NewCurrentPlayerStats)
{
	S2M_UpdateCurrentPlayerStats(NewCurrentPlayerStats);
}

void UPlayerStatusComponent::S2M_UpdateCurrentPlayerStats_Implementation(const FPlayerStats& NewCurrentPlayerStats)
{
	CurrentPlayerStats = NewCurrentPlayerStats;

	UpdateCurrentStatusUI();
}

void UPlayerStatusComponent::OnWeaponUpdate(const int32& UpdateEquipItemID, const bool& bIsEquip)
{
	CHECK_VALID(GI);

	FWeaponItemData UpdateWeaponItemData;
	if (!GI->GetWeaponItemInfo(UItemDataManager::GetPureID(UpdateEquipItemID), UpdateWeaponItemData))
	{
		return;
	}

	UpdateDefaultPlayerStats(bIsEquip, 0, 0, UpdateWeaponItemData.AttackPower, UpdateWeaponItemData.AttackSpeed, UpdateWeaponItemData.CriticalChance, 0);
}

void UPlayerStatusComponent::OnArmorUpdate(const EArmorType& UpdateArmorType, const int32& UpdateEquipItemID, const bool& bIsEquip)
{
	CHECK_VALID(GI);

	FArmorItemData UpdateArmorItemData;
	if (!GI->GetArmorItemInfo(UItemDataManager::GetPureID(UpdateEquipItemID), UpdateArmorItemData))
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
	
	C2S_UpdateDefaultPlayerStats(DefaultPlayerStats);
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

	if (PlayerChar->HasAuthority())
	{
		S2M_UpdateCurrentPlayerStats(CurrentPlayerStats);
	}
	else
	{
		C2S_UpdateCurrentPlayerStats(CurrentPlayerStats);
	}
}
