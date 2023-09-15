// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define CHECK_VALID(Obj) \
if (!(Obj)) \
{ \
    UE_LOG(LogTemp, Error, TEXT("NULL =  %s : %s"), *GetClass()->GetName(), TEXT(#Obj)); \
    return; \
}