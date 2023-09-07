// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Looting.h"
#include "FallingHellgate.h"
#include "Kismet/GameplayStatics.h"
#include "ItemDataManager.h"
#include "FHGameInstance.h"
#include "FHPlayerCharacter.h"
#include "FHPlayerController.h"
#include "InventoryComponent.h"

UAnimNotify_Looting::UAnimNotify_Looting(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShouldFireInEditor = false;
}

void UAnimNotify_Looting::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AFHPlayerController* PC = Cast<AFHPlayerController>(MeshComp->GetOwner()->GetInstigatorController());
	CHECK_VALID(PC);
	
	// Only Run in Local Client
	if (PC != UGameplayStatics::GetPlayerController(MeshComp->GetWorld(), 0))
	{
		return;
	}
	
	UFHGameInstance* GI = PC->GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	int32 CurrentDungeonID = GI->GetCurrentDungeonID();

	int32 LootItemID = GetRandomItemOnItemDropTable(CurrentDungeonID);

	PC->GetInventoryComp()->AddItemToInventory(LootItemID, 1);

	DestroyLootItem(MeshComp->GetOwner());
}

int32 UAnimNotify_Looting::GetRandomItemOnItemDropTable(const int32& DungeonID)
{
	FItemDropData* ItemDropData = GetItemDropData(DungeonID);
	check(ItemDropData);

	int32 TotDropWeight = GetTotalItemDropWeight(ItemDropData->ItemDropWeightsMap);

	// Replace the given weight with a percentage (DropWeightByItem / TotDropWeight)
	TMap<int32, float> NewItemDropWeightMap;
	for (auto& DropWeightByItem : ItemDropData->ItemDropWeightsMap)
	{
		NewItemDropWeightMap.Add(DropWeightByItem.Key, static_cast<float>(DropWeightByItem.Value) / TotDropWeight);
	}

	// Sort ItemDropMap by Weight
	NewItemDropWeightMap.ValueSort([](const float& A, const float& B) { return A < B; });

	// get 0.f ~ 1.f random point
	float RandomPoint = FMath::FRand();

	int32 ItemID = 0;
	float AccumulatedWeight = 0.f;

	for (const auto& DropWeightByItem : NewItemDropWeightMap)
	{
		AccumulatedWeight += DropWeightByItem.Value;

		if (RandomPoint <= AccumulatedWeight)
		{
			ItemID = DropWeightByItem.Key;
			break;
		}
	}

	return ItemID;
}

FItemDropData* UAnimNotify_Looting::GetItemDropData(const int32& DungeonID)
{
	check(ItemDropDataTable);

	return ItemDropDataTable->FindRow<FItemDropData>(*FString::FromInt(DungeonID), TEXT(""));
}

int32 UAnimNotify_Looting::GetTotalItemDropWeight(TMap<int32, int32>& InItemDropWeightsMap)
{
	int32 TotalItemDropWeights = 0;
	for (const auto& ItemDropWeight : InItemDropWeightsMap)
	{
		TotalItemDropWeights += ItemDropWeight.Value;
	}

	return TotalItemDropWeights;
}

void UAnimNotify_Looting::DestroyLootItem(AActor* Owner)
{
	if (AFHPlayerCharacter* Char = Cast<AFHPlayerCharacter>(Owner))
	{
		if (Char->LootingEffect)
		{
			Char->LootingEffect->Destroy();
		}

		Char->InteractingActor->Destroy();
		Char->InteractingActor = nullptr;
	}
}
