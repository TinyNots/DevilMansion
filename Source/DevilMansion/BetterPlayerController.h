// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BetterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DEVILMANSION_API ABetterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> WEnemyHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* EnemyHealthBar;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Health")
	bool bEnemyHealthBarVisible;

	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();

	FVector EnemyLocation;

	void SetHealthBarSize(FVector2D SizeInViewPort);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	float EnemyHeight;

	UFUNCTION()
	void SetEnemyHeight(float Height);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float GapBetweenEnemyAndHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float CenterPointOffset;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
