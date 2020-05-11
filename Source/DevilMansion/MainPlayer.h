// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPlayer.generated.h"

UCLASS()
class DEVILMANSION_API AMainPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,Category = "Mesh")
	class USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "PlayerMovement")
	float MaxSpeed;

private:
	void MoveX(float Value);
	void MoveY(float Value);

	FVector CurrentVelocity;
};
