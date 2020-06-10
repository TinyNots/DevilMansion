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

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "GameMode")
	ABetterPlayer* Player;


private:
	UPROPERTY()
	class UMyGameInstance* Instance;
};
