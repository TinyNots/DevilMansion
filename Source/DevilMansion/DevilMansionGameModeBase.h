// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BetterPlayer.h"
#include "DevilMansionGameModeBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGamePlayState :uint8
{
	ETitle UMETA(DeplayName = "Title"), 
	EPlaying UMETA(DeplayName = "Playing"),
	EGameOver UMETA(DeplayName = "GameOver"),
	EMAX UMETA(DeplayName = "DefaultMax"),
};

UCLASS()
class DEVILMANSION_API ADevilMansionGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ADevilMansionGameModeBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	ABetterPlayer* Player;

	// Returns the current playing state 
	UFUNCTION(BlueprintPure)
		EGamePlayState GetCurrentState() const;

	// Sets a new playing state 
	UFUNCTION(BlueprintCallable)
	void SetCurrentState(EGamePlayState NewState);

private:
	//Keeps track of the current playing state 
	EGamePlayState CurrentState;

	//Handle any function calls that rely upon changing the playing state of our game 
	void HandleNewState(EGamePlayState NewState);
};
