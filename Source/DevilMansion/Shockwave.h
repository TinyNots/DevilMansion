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
	class UStaticMeshComponent* StaticMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float WaveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float MaxRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SphereRadiusSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float OriginalRaidusSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Timer;
};
