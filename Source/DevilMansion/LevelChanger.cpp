// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChanger.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelChanger::ALevelChanger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NextLevelToLoad = "CharacterScene";
}

// Called when the game starts or when spawned
void ALevelChanger::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALevelChanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelChanger::LoadLevel()
{
	if (UGameplayStatics::GetCurrentLevelName(this) == "TitleScene")
	{
		NextLevelToLoad = "CharacterScene";
	}
	else if (UGameplayStatics::GetCurrentLevelName(this) == "CharacterScene")
	{
		NextLevelToLoad = "Map1";
	}
	else if (UGameplayStatics::GetCurrentLevelName(this) == "Map1")
	{
		NextLevelToLoad = "ResultScene";
	}
	else if (UGameplayStatics::GetCurrentLevelName(this) == "ResultScene")
	{
		NextLevelToLoad = "TitleScene";
	}

	UGameplayStatics::OpenLevel(this, NextLevelToLoad);
}