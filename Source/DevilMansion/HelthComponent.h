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
	UFUNCTION(BlueprintCallable)
	float GetPercent();
	UFUNCTION(BlueprintCallable)
	UHelthComponent* GetHealthSystem();

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Health")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	bool bHealthBar;
};
