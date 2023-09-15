// Fill out your copyright notice in the Description page of Project Settings.

#include "OnDragWidget.h"
#include "Components/Image.h"

void UOnDragWidget::SetItemImage(UTexture2D* Image)
{
	ItemImage->SetBrushFromTexture(Image);
}
