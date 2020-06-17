// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class DEVILMANSION_API AMainHUD : public AHUD
{
	GENERATED_BODY()
public:
	AMainHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere )
		TSubclassOf<class UUserWidget> MainHUDWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> TitleHUDWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> ResultHUDWidget;

	UPROPERTY(EditAnywhere)
		class UUserWidget* CurrentWidget;
};
