// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "DevilMansionGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MainHUD.h"


ADevilMansionGameModeBase::ADevilMansionGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	HUDClass = AMainHUD::StaticClass();
}

void ADevilMansionGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentState(EGamePlayState::EPlaying);

	Player = Cast<ABetterPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
}
//
void ADevilMansionGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetWorld()->GetMapName();

	if (Player)
	{
		if (FMath::IsNearlyZero(Player->GetHealth(), 0.001f))
		{
			//SetCurrentState(EGamePlayState::EGameOver);
		}
	}
}

EGamePlayState ADevilMansionGameModeBase::GetCurrentState() const
{
	return CurrentState;
}

void ADevilMansionGameModeBase::SetCurrentState(EGamePlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void ADevilMansionGameModeBase::HandleNewState(EGamePlayState NewState)
{
	switch (NewState)
	{
	case EGamePlayState::EPlaying:
	{
		// do nothing
	}
	break;
	// Unknown/default state
	case EGamePlayState::EGameOver:
	{
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}
	break;
	// Unknown/default state
	default:
	case EGamePlayState::EUnknown:
	{
		// do nothing
	}
	break;
	}
}