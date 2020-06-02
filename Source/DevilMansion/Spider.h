// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BadGuy.h"
#include "Spider.generated.h"

UCLASS()
class DEVILMANSION_API ASpider : public ABadGuy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpider();

	virtual float ResetAttackTimer() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float AttackTimerMax;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
