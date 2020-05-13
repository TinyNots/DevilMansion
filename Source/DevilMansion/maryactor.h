// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "maryactor.generated.h"

UCLASS()
class DEVILMANSION_API Amaryactor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Amaryactor();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	float MovementSpeed;

	UPROPERTY(EditAnywhere)
	float AttackDamage;

	UPROPERTY(EditAnywhere)
	float HitPoint;

	UPROPERTY(EditAnywhere)
	float AttackCooldown;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
