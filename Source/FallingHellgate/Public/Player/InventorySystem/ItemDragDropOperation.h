// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class FALLINGHELLGATE_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Drag Drop Operation")
	class UItemData* DraggingItemData;

	UPROPERTY(BlueprintReadOnly, Category = "Drag Drop Operation")
	int32 DraggingItemAmount;

};
