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

		
}
//
void ADevilMansionGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (CurrentState == EGamePlayState::EPlaying)
	{
		if (!Player)
		{
			Player = Cast<ABetterPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
		}
	}
	if (Player)
	{
		if (FMath::IsNearlyZero(Player->GetHealth(), 0.001f))
		{
			SetCurrentState(EGamePlayState::EGameOver);
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

	case EGamePlayState::ETitle:
	{
		UGameplayStatics::OpenLevel(this, "TitleScene");
		break;

	}
	case EGamePlayState::EPlaying:
	{
		UGameplayStatics::OpenLevel(this, "Map1");
		break;

	}
	// Unknown/default state
	case EGamePlayState::EGameOver:
	{
		UGameplayStatics::OpenLevel(this, "ResultScene");
		break;

	}
	// Unknown/default state
	default:
	{
		break;
	}
	}
}