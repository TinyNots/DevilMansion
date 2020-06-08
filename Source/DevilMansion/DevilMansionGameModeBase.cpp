// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "DevilMansionGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MainHUD.h"
#include "MyGameInstance.h"



ADevilMansionGameModeBase::ADevilMansionGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	HUDClass = AMainHUD::StaticClass();
}

void ADevilMansionGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Instance = UMyGameInstance::GetInstance();
		
}
//
void ADevilMansionGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Instance)
	{

		if (Instance->GetCurrentState() == EGamePlayState::EPlaying)
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
				Instance->SetCurrentState(EGamePlayState::EGameOver);
			}
		}
	}
}

