// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactive.generated.h"

UCLASS()
class DEVILMANSION_API AInteractive : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractive();

	//Overlap volume for functionality to be triggered
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactive")
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactive")
	class UBoxComponent* TriggerBoxLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactive")
	class UBoxComponent* TriggerBoxRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactive")
	class USkeletalMeshComponent* Door;

	UPROPERTY()
	float Delta;

	UPROPERTY()
	float TargetRotation;

	UPROPERTY()
	float CameraTargetRotation;

	UPROPERTY()
	AActor* CameraOwner;

	UPROPERTY()
	FRotator OriginalRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactive")
	float RotateRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraRotate")
	float CameraRotateLeft;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "CameraRotate")
	float CameraRotateRight;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void TriggerBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void TriggerBoxLeftOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void TriggerBoxRightOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Category = "Interact")
	void InteractDoorOpen(float Rotation);
};
