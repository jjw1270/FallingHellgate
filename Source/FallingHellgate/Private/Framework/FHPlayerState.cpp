// Fill out your copyright notice in the Description page of Project Settings.


#include "FHPlayerState.h"
#include "FallingHellgate.h"
#include "FHGameInstance.h"
#include "Net/UnrealNetwork.h"

AFHPlayerState::AFHPlayerState()
{

}

void AFHPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const

{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFHPlayerState, UserName);

}

void AFHPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority() && GetLocalRole() != ENetRole::ROLE_AutonomousProxy)
	{
		return;
	}

	UFHGameInstance* GI = GetGameInstance<UFHGameInstance>();
	CHECK_VALID(GI);

	int32 RandomID = FMath::RandRange(100, 999);

	SetUserName(FText::AsNumber(RandomID));
}

void AFHPlayerState::SetUserName(const FText& NewName)
{
	UserName = NewName;
}

void AFHPlayerState::OnRep_UserName()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SetUserName %s"), *UserName.ToString()));

	// broadcast for nametag
}
