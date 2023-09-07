// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "ItemDataManager.generated.h"

// Item drop data by dungeon struct
USTRUCT(BlueprintType)
struct FItemDropData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	FItemDropData()
		: DungeonID(0), DungeonName(TEXT("")), ItemDropWeightsMap()
	{
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 DungeonID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FString DungeonName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TMap<int32, int32> ItemDropWeightsMap;
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None            UMETA(DisplayName = "None"),
	Consumable      UMETA(DisplayName = "Consumable"),
	Weapon			UMETA(DisplayName = "Weapon"),
	Armor			UMETA(DisplayName = "Armor"),
	Others			UMETA(DisplayName = "Others"),
};

USTRUCT(BlueprintType)
struct FBaseItemData
{
	GENERATED_USTRUCT_BODY()

public:
	FBaseItemData()
		: ID(0), Name(TEXT("")), Type(EItemType::None), BasePrice(0), UpgradeValue(0), TextInfo(TEXT("")), Icon2D(nullptr)
	{
	}
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    int32 ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EItemType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    int32 BasePrice;

	UPROPERTY(BlueprintReadOnly, Category = Data)
	int32 UpgradeValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    FString TextInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    UTexture2D* Icon2D;

public:
	bool IsValid() const
	{
		return ID != 0;
	}
};

UENUM(BlueprintType)
enum class EEffectTarget : uint8
{
	None				UMETA(DisplayName = "None"),
	Health				UMETA(DisplayName = "Health"),
	Stamina				UMETA(DisplayName = "Stamina"),
	AttackPower			UMETA(DisplayName = "AttackPower"),
	AttackSpeed			UMETA(DisplayName = "AttackSpeed"),
	CriticalChance		UMETA(DisplayName = "CriticalChance"),
	DefensivePower		UMETA(DisplayName = "DefensivePower"),
};

USTRUCT(BlueprintType)
struct FConsumableItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	FConsumableItemData()
		: BaseData(), bIsEffectSelf(false), Duration(0.0f), EffectTarget(EEffectTarget::None), EffectValue(0.0f)
	{
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FBaseItemData BaseData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	bool bIsEffectSelf;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EEffectTarget EffectTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float EffectValue;

public:
	bool IsValid() const
	{
		return BaseData.IsValid();
	}
};

USTRUCT(BlueprintType)
struct FWeaponItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FWeaponItemData()
		: BaseData(), StaminaUsage(0), AttackPower(0), AttackSpeed(0.0f), CriticalChance(0.0f), WeaponMesh(nullptr)
	{
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FBaseItemData BaseData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 StaminaUsage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 AttackPower;

	// Default : 1.0f, Max : 2.5f
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float AttackSpeed;

	// Max : 1.0f
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float CriticalChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	class USkeletalMesh* WeaponMesh;

public:
	bool IsValid() const
	{
		return BaseData.IsValid();
	}
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	None        UMETA(DisplayName = "None"),
	Helmet      UMETA(DisplayName = "Helmet"),
	Upper       UMETA(DisplayName = "Upper"),
	Gloves		UMETA(DisplayName = "Gloves"),
	Lower       UMETA(DisplayName = "Lower"),
	Shoes       UMETA(DisplayName = "Shoes"),

};

USTRUCT(BlueprintType)
struct FArmorItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:
	FArmorItemData()
		: BaseData(), ArmorType(EArmorType::None), Health(0), Stamina(0), AttackPower(0),
		  AttackSpeed(0.0f), CriticalChance(0.0f), DefensivePower(0),
		  ArmorMesh(nullptr), bHideHairWhenVisible(false), bShouldSwitchBetweenAdditionalMesh(false), AdditionalArmorMesh(nullptr)
	{
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	FBaseItemData BaseData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	EArmorType ArmorType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 AttackPower;

	// Default : 1.0f, Max : 2.5f
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float AttackSpeed;

	// Max : 1.0f
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float CriticalChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	int32 DefensivePower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	class USkeletalMesh* ArmorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	bool bHideHairWhenVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	bool bShouldSwitchBetweenAdditionalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	class USkeletalMesh* AdditionalArmorMesh;

public:
	bool IsValid() const
	{
		return BaseData.IsValid();
	}
};

UCLASS(BlueprintType, Blueprintable)
class FALLINGHELLGATE_API UItemDataManager : public UObject
{
	GENERATED_BODY()

	//Item DataTables
protected:
	UPROPERTY(EditDefaultsOnly, Category = Data)
	class UDataTable* ConsumableItemDataTable;

	UPROPERTY(EditDefaultsOnly, Category = Data)
	class UDataTable* WeaponItemDataTable;

	UPROPERTY(EditDefaultsOnly, Category = Data)
	class UDataTable* ArmorItemDataTable;

public:
	UFUNCTION(BlueprintCallable, Category = Data)
	bool GetConsumableItemInfo(const int32& ItemID, FConsumableItemData& OutData);

	UFUNCTION(BlueprintCallable, Category = Data)
	bool GetWeaponItemInfo(const int32& ItemID, FWeaponItemData& OutData);

	UFUNCTION(BlueprintCallable, Category = Data)
	bool GetArmorItemInfo(const int32& ItemID, FArmorItemData& OutData);

	UFUNCTION(BlueprintCallable, Category = Data)
	EItemType GetItemType(const int32& ItemID);
};
