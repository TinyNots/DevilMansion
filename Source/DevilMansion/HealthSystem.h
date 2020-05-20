// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DEVILMANSION_API HealthSystem
{
public:
	HealthSystem();
	~HealthSystem();
	void SetHealth(float Value);
	float GetHealth();
	void ShowHealthBar(bool Flag);
	bool IncreaseHealth(float Value);
	bool DecreaseHealth(float Value);
	float GetPercent();
	HealthSystem* GetHealthSystem();
private:
	float Health;
	float MaxHealth;
	bool bHealthBar;
};
