// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEVILMANSION_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()
public:
protected:
public:
	UFUNCTION(BlueprintCallable)
		void SetHealth(float Value);
	UFUNCTION(BlueprintCallable)
		float GetHealth();
	UFUNCTION(BlueprintCallable)
		void ShowHealthBar(bool Flag);
	UFUNCTION(BlueprintCallable)
		bool IncreaseHealth(float Value);
	UFUNCTION(BlueprintCallable)
		bool DecreaseHealth(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
		float Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
		bool bHealthBar;
};
