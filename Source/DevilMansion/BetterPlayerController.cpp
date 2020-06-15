// Fill out your copyright notice in the Description page of Project Settings.


#include "BetterPlayerController.h"
#include "Blueprint/UserWidget.h"

void ABetterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		FVector2D Alignment(0.0f, 0.0f);
		EnemyHealthBar->SetAlignmentInViewport(Alignment);
	}
}

void ABetterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyHealthBar)
	{
		FVector2D PositionInViewport;
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);

		//FVector2D SizeInViewport = FVector2D(200.0f, 25.0f);
		PositionInViewport -= FVector2D(CenterPointOffset, (EnemyHeight / 2.0f) - GapBetweenEnemyAndHealth);
		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		//EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);
	}
}

void ABetterPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void ABetterPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABetterPlayerController::SetHealthBarSize(FVector2D SizeInViewPort)
{
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewPort);
	}
}

void ABetterPlayerController::SetEnemyHeight(float Height)
{
	EnemyHeight = Height;
}