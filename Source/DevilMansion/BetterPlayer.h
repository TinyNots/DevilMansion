// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BetterPlayer.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;

UCLASS()
class DEVILMANSION_API ABetterPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABetterPlayer();

	// Camera boom positioning the camera behind the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAcess = "true"))
	class USpringArmComponent* CameraBoom;

	// Camera that follow the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAcess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* ItemCollisionVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* EnemyCollisionVolume;

	// Base turn rates to scale turning functions for the camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class AObjectOutline*> HighlightActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* Face;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* Hair;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* Gloves;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* Shoes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* HeadGears;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* ShoulderPad;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* Belt;

	// Called for forwards and backwrards input
	void MoveForward(float Value);

	// Called for side to side input
	void MoveSide(float Value);

	// Called via input to turn at a given rate
	void TurnAtRate(float Rate);

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Anims")
	bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int ComboCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class AWeapon* EquippedWeapon;

	FORCEINLINE void SetEquippedWeapon(AWeapon* WeaponToSet) { EquippedWeapon = WeaponToSet; }
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }

	void DebugEquip();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UStaticMesh* DebugWeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	int MaxComboCount;

	void Defend();
	void DefendEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bDefending;

	UFUNCTION()
	void OutlineCheck(class USphereComponent* CollisionVolume,int objectTypeIdx);

	void Pickup();

	// Interation with Door and Camera
	UFUNCTION(Category = "Interact")
	void InteractStart(float TargetRotation, bool Boolean, AActor* Door);
	UFUNCTION(Category = "Interact")
	void InteractStartSwitch(bool Boolean, AActor* Switch);

	UFUNCTION(Category = "Interact")
	void CameraRotatable(bool Boolean, AActor* Rotator);

	UFUNCTION(Category = "Interact")
	AActor* GetLastRotator();
private:
	UPROPERTY()
	UAnimInstance* AnimInstance;

	UPROPERTY()
	float Health;

	//Mary's
	UPROPERTY()
	float DoorOpenRotate;
	UPROPERTY()
	bool bDoorNearby;
	UPROPERTY()
	bool bIsCameraRotating;
	UPROPERTY()
	AActor* InteractingDoor;
	UPROPERTY()
	AActor* InteractingRotator;
	UPROPERTY()
	AActor* InteractingSwitch;
	UPROPERTY()
	bool bSwitchNearby;

	void Skill();
};
