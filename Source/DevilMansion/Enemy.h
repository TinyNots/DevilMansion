// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class DEVILMANSION_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere)
	float Movespeed;
	UPROPERTY(EditAnywhere)
	float Damage;
	UPROPERTY(EditAnywhere)
	float Cooldown;
	UPROPERTY(EditAnywhere)
	float Hitpoint;

	/*UPROPERTY(EditDefaultsOnly, Category = "Item")
		TSubclassOf<class AObjectOutline> Outline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		uint8 OutlineMaterialIndex;

	UPROPERTY()
		AObjectOutline* OutlineRef;*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
