// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Torch.generated.h"

UCLASS()
class DEVILMANSION_API ATorch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATorch();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
	class USphereComponent* CollisionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
	class UParticleSystem* FireParticles;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision | Particles")
	bool bLightUp;

	//lighted up fire position when player overlap with trigger collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
	TArray<struct FVector> FirePosition;

	//register to fog of war manager if it has been lighted up 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> FowManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRegisterToFow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPlaySound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SpawnFire();

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
