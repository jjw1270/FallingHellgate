// Fill out your copyright notice in the Description page of Project Settings.


#include "FHGameInstance.h"
#include "FallingHellgate.h"
#include "ItemData.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

UFHGameInstance::UFHGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

// If Save Data is Valid, Load Save Data
void UFHGameInstance::Init()
{
	Super::Init();

	if (ItemDataManagerClass)
	{
		ItemDataManager = NewObject<UItemDataManager>(this, ItemDataManagerClass);
	}
}

// Save Data when End Game
void UFHGameInstance::Shutdown()
{

	Super::Shutdown();
}

void UFHGameInstance::TESTss()
{
	UE_LOG(LogTemp, Warning, TEXT("<<<<Inventory Items>>>>"));
	for (const auto& Item : InventoryItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : %d [%d]"), *Item.Key->GetBaseData().Name, Item.Value, Item.Key->GetUniqueID());
	}
	UE_LOG(LogTemp, Warning, TEXT("--------------------------------------------"));

	UE_LOG(LogTemp, Warning, TEXT("<<<<QuickSlot Items>>>>"));
	for (const auto& Item : QuickSlotItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("Index[%d] : %s"), Item.Key, *Item.Value->GetBaseData().Name);
	}
	UE_LOG(LogTemp, Warning, TEXT("--------------------------------------------"));

	UE_LOG(LogTemp, Warning, TEXT("<<<<Equipment Items>>>>"));
	for (const auto& Item : EquipmentItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s [%d]"), *Item->GetBaseData().Name, Item->GetUniqueID());
	}
	UE_LOG(LogTemp, Warning, TEXT("--------------------------------------------"));

	UE_LOG(LogTemp, Warning, TEXT("<<<<Default Player Stats>>>>"));
	UE_LOG(LogTemp, Warning, TEXT("Health : [%d]"), DefaultPlayerStats.Health);
	UE_LOG(LogTemp, Warning, TEXT("Stamina : [%d]"), DefaultPlayerStats.Stamina);
	UE_LOG(LogTemp, Warning, TEXT("Attack : [%d]"), DefaultPlayerStats.Attack);
	UE_LOG(LogTemp, Warning, TEXT("AttackSpeed : [%f]"), DefaultPlayerStats.AttackSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Critical : [%f]"), DefaultPlayerStats.Critcal);
	UE_LOG(LogTemp, Warning, TEXT("Defence : [%d]"), DefaultPlayerStats.Defence);
	UE_LOG(LogTemp, Warning, TEXT("--------------------------------------------"));

}
