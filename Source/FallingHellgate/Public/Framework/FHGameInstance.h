// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FHGameInstance.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FDefaultPlayerStats
{
	GENERATED_USTRUCT_BODY()

public:
	FDefaultPlayerStats()
		: DefaultHealth(1000), DefaultStamina(1000), DefaultAttack(0),
		DefaultAttackSpeed(0.f), DefaultCritcal(0.f), DefaultDefence(0)
	{
	}

public:
	UPROPERTY(BlueprintReadOnly, Category = Status)
	int32 DefaultHealth;

	UPROPERTY(BlueprintReadOnly, Category = Status)
	int32 DefaultStamina;

	UPROPERTY(BlueprintReadOnly, Category = Status)
	int32 DefaultAttack;

	UPROPERTY(BlueprintReadOnly, Category = Status)
	float DefaultAttackSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Status)
	float DefaultCritcal;

	UPROPERTY(BlueprintReadOnly, Category = Status)
	int32 DefaultDefence;
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
	FDefaultPlayerStats DefaultPlayerStats;

public:
	UFUNCTION(BlueprintCallable, Category = Data)
	FORCEINLINE FText& GetPlayerName() { return PlayerName; }

	UFUNCTION(BlueprintCallable, Category = Status)
	FORCEINLINE int32& GetPlayerMoney() { return Money; }

	UFUNCTION(BlueprintCallable, Category = Status)
	FORCEINLINE FDefaultPlayerStats& GetDefaultPlayerStats() { return DefaultPlayerStats; }
};
