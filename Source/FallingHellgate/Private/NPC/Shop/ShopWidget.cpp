// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"
#include "Components/Button.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_CloseShop->OnClicked.AddDynamic(this, &UShopWidget::OnCloseShopClicked);
}

void UShopWidget::OnCloseShopClicked()
{

}
