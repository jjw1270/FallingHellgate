#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

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
struct FBaseItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EItemType Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	int32 BasePrice;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	int32 MaxUpgradeValue{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FText TextInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
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

struct FConsumableItemData : public FBaseItemData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	bool bIsEffectSelf{ true };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EEffectTarget EffectTarget{ EEffectTarget::None };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	float EffectValue;
};

struct FWeaponItemData : public FBaseItemData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	int32 StaminaUsage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	int32 AttackPower;

	// Max : 2.5f
	// Add
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	float AttackSpeed{ 1.0 };

	// Max : 1.0f
	// Add
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	float CriticalChance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	class USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	class UAnimMontage* NormalAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	class UAnimMontage* SmashAttackMontage;
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

UENUM(BlueprintType)
enum class EArmorSwapableType : uint8
{
	None		UMETA(DisplayName = "None"),
	HideHair	UMETA(DisplayName = "HideHair"),
	SwitchMesh	UMETA(DisplayName = "SwitchMesh"),
};

struct FArmorItemData : public FBaseItemData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EArmorType ArmorType{ EArmorType::None };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	int32 Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	int32 AttackPower;

	// Default : 1.0f, Max : 2.5f
	// Add
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	float AttackSpeed;

	// Max : 1.0f
	// Add
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	float CriticalChance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	int32 DefensivePower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	class USkeletalMesh* ArmorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EArmorSwapableType ArmorSwapableType{ EArmorSwapableType::None };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	class USkeletalMesh* SwitchableArmorMesh;
};
