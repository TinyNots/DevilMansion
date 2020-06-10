// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

#define NOITEM_DROP_RATE 100

UCLASS()
class DEVILMANSION_API AItem : public AActor
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	class USphereComponent* CollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	class UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	class UParticleSystemComponent* IdleParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
	class UParticleSystem* OverlapParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sounds")
	class USoundCue* OverlapSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bCanOutline;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<class AObjectOutline> Outline;

	//replace normal material index with outline material index
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	uint8 OutlineMaterialIndex;

	UPROPERTY()
	AObjectOutline* OutlineRef;

	//item drop rate as weight value (deafult as 100)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int ItemDropRate;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void PickupEffect();

	UFUNCTION()
	virtual void PickUp(class ABetterPlayer* Player);
};
