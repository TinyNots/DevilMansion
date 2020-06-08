// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Engine/Engine.h"
#include "DevilMansionGameModeBase.h"
#include "Kismet/GameplayStatics.h"


UMyGameInstance* UMyGameInstance::GetInstance()
{
    UMyGameInstance* instance = nullptr;

    if (GEngine)
    {
        FWorldContext* context = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport);
        instance = Cast<UMyGameInstance>(context->OwningGameInstance);

    }

    return instance;
}

EGamePlayState UMyGameInstance::GetCurrentState() const
{
	return CurrentState;
}

void UMyGameInstance::SetCurrentState(EGamePlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void UMyGameInstance::HandleNewState(EGamePlayState NewState)
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