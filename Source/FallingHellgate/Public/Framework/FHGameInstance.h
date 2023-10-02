// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FHGameInstance.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FPlayerStats
{
	GENERATED_USTRUCT_BODY()

public:
	FPlayerStats()
		: Health(1000), Stamina(1000), Attack(0),
		AttackSpeed(0.f), Critcal(0.f), Defence(0)
	{
	}

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	int32 Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	int32 Stamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	int32 Attack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	float AttackSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	float Critcal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	int32 Defence;
};

UCLASS()
class FALLINGHELLGATE_API UFHGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	//test
	UFUNCTION(BlueprintCallable, Category = Test)
	void TESTss();

protected:
	virtual void Init() override;

	virtual void Shutdown() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Data)
	TSubclassOf<class UItemDataManager> ItemDataManagerClass;

	UPROPERTY()
	class UItemDataManager* ItemDataManager;

public:
	UFUNCTION(BLueprintCallable)
	FORCEINLINE class UItemDataManager* GetItemDataManager() const { return ItemDataManager; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data)
	int32 CurrentDungeonID;

public:
	UFUNCTION(BlueprintCallable, Category = Data)
	FORCEINLINE void SetCurrentDungeonID(const int32& NewDungeonID) { CurrentDungeonID = NewDungeonID; }

	UFUNCTION(BlueprintCallable, Category = Data)
	FORCEINLINE int32 GetCurrentDungeonID() const { return CurrentDungeonID; }

protected:
	// TMap<ItemData, ItemAmount>
	UPROPERTY()
	TMap<class UItemData*, int32> InventoryItems;

	// TMap<QuickSlotIdx, ItemData>
	UPROPERTY()
	TMap<int32, class UItemData*> QuickSlotItems;

	// TArray<ItemData>
	UPROPERTY()
	TArray<class UItemData*> EquipmentItems;

public:
	FORCEINLINE TMap<class UItemData*, int32>* GetInventoryItems() { return &InventoryItems; }

	FORCEINLINE TMap<int32, class UItemData*>* GetQuickSlotItems() { return &QuickSlotItems; }

	FORCEINLINE TArray<class UItemData*>* GetEquipments() { return &EquipmentItems; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data)
	FText PlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	int32 Money;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	FPlayerStats DefaultPlayerStats;

public:
	UFUNCTION(BlueprintCallable, Category = Data)
	FORCEINLINE void SetPlayerName(const FText& NewPlayerName) { PlayerName = NewPlayerName; }

	UFUNCTION(BlueprintCallable, Category = Data)
	FORCEINLINE FText& GetPlayerName() { return PlayerName; }

	UFUNCTION(BlueprintCallable, Category = Status)
	FORCEINLINE int32& GetPlayerMoney() { return Money; }

	UFUNCTION(BlueprintCallable, Category = Status)
	FORCEINLINE FPlayerStats& GetDefaultPlayerStats() { return DefaultPlayerStats; }
};
