// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidget.h"

void UHealthWidget::SetHealth(float Value)
{
	MaxHealth = Value;
	Health = MaxHealth;
}

float UHealthWidget::GetHealth()
{
	return Health;
}

void UHealthWidget::ShowHealthBar(bool Flag)
{
	bHealthBar = Flag;
}

bool UHealthWidget::IncreaseHealth(float Value)
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

bool UHealthWidget::DecreaseHealth(float Value)
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

float UHealthWidget::GetPercent()
{
	return Health / MaxHealth;
}
