// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectOutline.generated.h"

UCLASS()
class DEVILMANSION_API AObjectOutline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectOutline();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Outline")
	class UStaticMeshComponent* VisualMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Outline")
	class USphereComponent* CollisionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outline")
	class UMaterial* OutlineMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Outline")
	bool bOutlining;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* SkeletalMesh;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Outline")
	//bool bIsNearestObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Outline")
	bool bCanPickup;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void Pickup();
};
