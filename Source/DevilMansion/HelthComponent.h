// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HelthComponent.generated.h"

/**
 * 
 */
UCLASS()
class DEVILMANSION_API UHelthComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UHelthComponent();
protected:
	virtual void BeginPlay() override;
public:
	UFUNCTION()
	void UpdateHealth();
};
