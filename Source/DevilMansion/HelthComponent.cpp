// Fill out your copyright notice in the Description page of Project Settings.


#include "HelthComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "HealthWidget.h"

UHelthComponent::UHelthComponent()
{
	/*HealthWidget->SetHealth(100.0f);
	SetWidget(HealthWidget);*/
}

void UHelthComponent::UpdateHealth()
{
	APlayerCameraManager *CamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector CamForward = CamManager->GetCameraRotation().Vector();
	FVector CamTrace = CamForward * -1;
	SetRelativeRotation(CamTrace.ToOrientationRotator());
}

void UHelthComponent::SetHealth(float Value)
{
	MaxHealth = Value;
	Health = MaxHealth;
}

float UHelthComponent::GetHealth()
{
	return 0.0f;
}

void UHelthComponent::ShowHealthBar(bool Flag)
{
	bHealthBar = Flag;
}

bool UHelthComponent::IncreaseHealth(float Value)
{
	if (Health < MaxHealth)
	{
		Health += Value;
		if (Health >= MaxHealth)
		{
			Health = MaxHealth;
			return true;
		}
	}
	return false;
}

bool UHelthComponent::DecreaseHealth(float Value)
{
	if (Health > 0.0f)
	{
		Health -= Value;
		if (Health < 0.0f)
		{
			Health = 0.0f;
			return true;
		}
	}
	return false;
}

float UHelthComponent::GetPercent()
{
	return Health / MaxHealth;
}

UHelthComponent * UHelthComponent::GetHealthSystem()
{
	return this;
}
