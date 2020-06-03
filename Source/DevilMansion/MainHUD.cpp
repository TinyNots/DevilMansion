// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"
#include "Blueprint/UserWidget.h"


AMainHUD::AMainHUD()
{

	//static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("/Game/FirstPerson/UI/Health_UI"));
	//HUDWidgetClass = HealthBarObj.Class;
}


void AMainHUD::DrawHUD()
{
	Super::DrawHUD();

}

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}