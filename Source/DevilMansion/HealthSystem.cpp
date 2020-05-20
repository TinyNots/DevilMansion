// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthSystem.h"

HealthSystem::HealthSystem()
{
	MaxHealth = 100.0f;
	Health = 100.0f;
	bHealthBar = false;
}

HealthSystem::~HealthSystem()
{
}

void HealthSystem::SetHealth(float Value)
{
	MaxHealth = Value;
	Health = MaxHealth;
}

float HealthSystem::GetHealth()
{
	return Health;
}

void HealthSystem::ShowHealthBar(bool Flag)
{
	bHealthBar = Flag;
}

bool HealthSystem::IncreaseHealth(float Value)
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

bool HealthSystem::DecreaseHealth(float Value)
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

float HealthSystem::GetPercent()
{
	return MaxHealth / Health;
}

HealthSystem* HealthSystem::GetHealthSystem()
{
	return this;
}
