// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraRotator.generated.h"

UCLASS()
class DEVILMANSION_API ACameraRotator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraRotator();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraRotator")
	class UBoxComponent* TriggerBox;

	UPROPERTY()
	AActor* CameraOwner;

	UPROPERTY()
	float CameraDelta;

	UPROPERTY()
	float OriginalCameraHeight;

	UPROPERTY()
	FRotator OriginalRotation;

	// Editable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraRotator")
	float TargetAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraRotator")
	float CameraHeight;

	//From 0.1 to 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraRotator")
	float CameraRotateRate;

	UFUNCTION(Category = "InteCameraRotatorract")
	void RotatorOverlap();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void TriggerBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
