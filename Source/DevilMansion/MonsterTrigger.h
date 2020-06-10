// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterTrigger.generated.h"

USTRUCT(BlueprintType)
struct FSpawnPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
	FVector SpawnPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
	bool ChasePlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
	TSubclassOf<AActor> TargetActor;
};

UCLASS()
class DEVILMANSION_API AMonsterTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterTrigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
	TArray<struct FSpawnPoint> Spawns;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TriggerPoint")
	class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnPoint")
	AActor* ActivationObject;

	UPROPERTY()
	int SpawnedEnemyCount;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool activated;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void TriggerBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SpawnedEnemyDeath();
};
