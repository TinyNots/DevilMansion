// Fill out your copyright notice in the Description page of Project Settings.


#include "BetterPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Containers/Array.h"
#include "Components/TextBlock.h"

void ABetterPlayerController::PlayWidgetAnimaiton(FString AnimationName)
{
	if (InfoWidget && WidgetAnimationMap.Contains(AnimationName) && !bIsShowingInfo)
	{
		InfoWidget->PlayAnimation(WidgetAnimationMap[AnimationName]);
	}
}

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

	bIsShowingInfo = false;
	if (ClassInfoWidget)
	{
		InfoWidget = CreateWidget<UUserWidget>(this, ClassInfoWidget);
		if (InfoWidget)
		{
			InfoWidget->AddToViewport();

			UWidgetBlueprintGeneratedClass* WidgetClass = InfoWidget->GetWidgetTreeOwningClass();
			for (int i = 0; i < WidgetClass->Animations.Num(); i++)
			{
				FString AnimationName = WidgetClass->Animations[i]->GetDisplayLabel();
				WidgetAnimationMap.Emplace(AnimationName, WidgetClass->Animations[i]);
			}

			UObjectProperty* ObjectProperty = Cast<UObjectProperty>(WidgetClass->FindPropertyByName("Text"));
			InfoTextBlock = Cast<UTextBlock>(ObjectProperty->GetObjectPropertyValue_InContainer(InfoWidget));
			if (!InfoTextBlock)
			{
				UE_LOG(LogTemp, Error, TEXT("InfoTextBlock fail to init."));
			}
		}
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