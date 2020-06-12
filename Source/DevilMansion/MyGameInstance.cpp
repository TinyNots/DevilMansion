// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Engine/Engine.h"
#include "DevilMansionGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Checkpoint.h"
#include "Torch.h"
#include "FogOfWarManager.h"
#include "ElevatorSwitch.h"
#include "BetterPlayer.h"

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

void UMyGameInstance::Save()
{
	UCheckpoint* SaveGameInstance = Cast<UCheckpoint>(UGameplayStatics::CreateSaveGameObject(UCheckpoint::StaticClass()));
	if (SaveGameInstance)
	{
		bIsSave = false;
		ABetterPlayer* Player = Cast<ABetterPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ABetterPlayer::StaticClass()));
		if (Player)
		{
			SaveGameInstance->SaveInfo.PlayerLocation = Player->GetActorLocation();
			SaveGameInstance->SaveInfo.PlayerRotation = Player->GetActorRotation();
			SaveGameInstance->SaveInfo.Health = Player->Health;
			SaveGameInstance->SaveInfo.MaxHealth = Player->MaxHealth;
			SaveGameInstance->SaveInfo.bWeapon = Player->bWeapon;
			//SaveGameInstance->SaveInfo.EquippedWeapon = EquippedWeapon;
			SaveGameInstance->SaveInfo.WeaponType = Player->WeaponType;
		}
		AFogOfWarManager* FOWMng = Cast<AFogOfWarManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AFogOfWarManager::StaticClass()));
		if (FOWMng)
		{
			UE_LOG(LogTemp, Warning, TEXT("FOWMng Found"));

			SaveGameInstance->SaveInfo.UnfoggedData = FOWMng->UnfoggedData;
		}
		TArray<AActor*> outLightActors;
		TArray<AActor*> outSwitchActors;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATorch::StaticClass(), outLightActors);
		for (auto& actor : outLightActors)
		{
			ATorch* torch = Cast<ATorch>(actor);
			SaveGameInstance->SaveInfo.LightedUpTorch.Add(torch->GetName(), torch->bLightUp);
		}
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AElevatorSwitch::StaticClass(), outSwitchActors);
		for (auto& actor : outSwitchActors)
		{
			AElevatorSwitch* switchs = Cast<AElevatorSwitch>(actor);
			SaveGameInstance->SaveInfo.DoorTriggerInfo.Add(switchs->GetName(), switchs->bActivated);
		}

		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex))
		{
			bIsSave = true;
			UE_LOG(LogTemp, Warning, TEXT("Saved"));

		}
		else
		{
			bIsSave = false;
		}
	}
}

void UMyGameInstance::Load()
{
	UCheckpoint* LoadedGame = Cast<UCheckpoint>(UGameplayStatics::LoadGameFromSlot(TEXT("Test"), 0));
	bIsLoad = false;
	if (LoadedGame)
	{
		// The operation was successful, so LoadedGame now contains the data we saved earlier.
		bIsLoad = true;

		ABetterPlayer* Player = Cast<ABetterPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ABetterPlayer::StaticClass()));
		if (Player)
		{
			Player->SetActorLocation(LoadedGame->SaveInfo.PlayerLocation);
			Player->SetActorRotation(LoadedGame->SaveInfo.PlayerRotation);
			Player->Health = LoadedGame->SaveInfo.Health;
			Player->MaxHealth = LoadedGame->SaveInfo.MaxHealth;
			Player->bWeapon = LoadedGame->SaveInfo.bWeapon;
			//EquippedWeapon = LoadedGame->SaveInfo.EquippedWeapon;
			Player->WeaponType = LoadedGame->SaveInfo.WeaponType;
		}

		TArray<AActor*> outActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATorch::StaticClass(), outActors);
		for (auto& actor : outActors)
		{
			ATorch* torch = Cast<ATorch>(actor);
			for (auto& torchdata : LoadedGame->SaveInfo.LightedUpTorch)
			{
				if (torch->GetName() == torchdata.Key)
				{
					torch->bLightUp = torchdata.Value;
					if (torch->bLightUp)
					{
						torch->bPlaySound = true;
						torch->SpawnFire();
					}
					break;
				}
			}
		}

		TArray<AActor*> outSwitchActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AElevatorSwitch::StaticClass(), outSwitchActors);
		for (auto& actor : outSwitchActors)
		{
			AElevatorSwitch* switchs = Cast<AElevatorSwitch>(actor);
			for (auto& switchdata : LoadedGame->SaveInfo.DoorTriggerInfo)
			{
				if (switchs->GetName() == switchdata.Key)
				{
					switchs->bActivated = switchdata.Value;
					if (switchs->bActivated)
					{
						switchs->ActivateSwitch();
					}
					break;
				}
			}
		}


		AFogOfWarManager* FOWMng = Cast<AFogOfWarManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AFogOfWarManager::StaticClass()));
		if (FOWMng)
		{
			UE_LOG(LogTemp, Warning, TEXT("FOWMng Found"));
			FOWMng->UnfoggedData.Init(false, FOWMng->TextureSize * FOWMng->TextureSize);
			FOWMng->UnfoggedData = LoadedGame->SaveInfo.UnfoggedData;
		}
		UE_LOG(LogTemp, Warning, TEXT("Loaded"));
	}
	else
	{
		bIsLoad = false;
	}
}
