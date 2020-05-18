// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ModularPlayer.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;

UCLASS()
class DEVILMANSION_API AModularPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AModularPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* Face;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* Hair;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* Gloves;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* Shoes;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* HeadGears;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* ShoulderPad;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* Belt;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* RightWeapon;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* LeftWeapon;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* Shield;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* Backpack;

	UFUNCTION()
	void AttachMeshToSocket(USkeletalMeshComponent* SkeletalMesh,FName SocketName);
	void AttachMeshToSocket(UStaticMeshComponent* StaticMesh,FName SocketName);
};
