// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"
#include "Blueprint/UserWidget.h"
#include "MyGameInstance.h"
#include "DevilMansionGameModeBase.h"


AMainHUD::AMainHUD()
{
}


void AMainHUD::DrawHUD()
{
	Super::DrawHUD();

}

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	UMyGameInstance* gameInst = UMyGameInstance::GetInstance();


	switch (gameInst->GetCurrentState())
	{

	case EGamePlayState::ETitle:
	{

		if (TitleHUDWidget != nullptr)
		{
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), TitleHUDWidget);
		}
		break;

	}
	case EGamePlayState::EPlaying:
	{
		if (MainHUDWidget != nullptr)
		{
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MainHUDWidget);
		}
		break;

	}
	// Unknown/default state
	case EGamePlayState::EGameOver:
	{
		if (ResultHUDWidget != nullptr)
		{
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), ResultHUDWidget);
		}
		break;
	}
	// Unknown/default state
	default:
	{
		break;
	}
	}
	
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport();
	}
	
}