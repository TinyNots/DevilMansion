// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameFramework/Actor.h"
#include "ElevatorSwitch.generated.h"

UENUM(BlueprintType)
enum class ESwitchType :uint8
{
	EST_UP UMETA(DeplayName = "Up"),
	EST_DOWN UMETA(DeplayName = "Down"),
	EST_LEFT UMETA(DeplayName = "Left"),
	EST_RIGHT UMETA(DeplayName = "Right"),
	EMS_MAX UMETA(DeplayName = "DefaultMax"),
};


UCLASS()
class DEVILMANSION_API AElevatorSwitch : public AItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevatorSwitch();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* TriggerSphere;

	UPROPERTY(BlueprintReadOnly)
	bool bTickEnabled;

	UPROPERTY(BlueprintReadOnly)
	bool bEnabled;

	UPROPERTY(BlueprintReadOnly)
	bool bActivated;

	UPROPERTY(BlueprintReadOnly)
	float MovedDistance;

	// Visible
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorSwitch")
	AActor* TargetObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorSwitch")
	ESwitchType SwitchType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorSwitch")
	bool bEnableByDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorSwitch")
	float TargetMoveDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorSwitch")
	float MoveRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorSwitch")
	float SwitchActivateSpinRate;

	UPROPERTY(EditAnywhere, Category = "ElevatorSwitch")
	class USoundBase* LoopSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ElevatorSwitch")
	float SoundVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info Text")
	FText InfoText;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ActivateSwitch();

	UFUNCTION()
	virtual void TriggerSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void TriggerSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
