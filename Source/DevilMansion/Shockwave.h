// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shockwave.generated.h"

UCLASS()
class DEVILMANSION_API AShockwave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShockwave();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* WaveCollision;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float WaveSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	float WaveTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float WaveTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float ForcePower;
};
