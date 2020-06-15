// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "BetterPlayer.generated.h"


class USkeletalMeshComponent;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EEquippedWeapon :uint8
{
	EMS_LeftEquippedWeapon UMETA(DeplayName = "LeftEquippedWeapon"),
	EMS_RightEquippedWeapon UMETA(DeplayName = "RightEquippedWeapon"),
	EMS_MAX UMETA(DeplayName = "DefaultMax")
};

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


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

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

	void Roll();

	UFUNCTION(BlueprintCallable)
		void RollEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float RollForce;

	UFUNCTION(BlueprintCallable)
		void AttackEnd();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
		bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		bool bCombo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		int ComboCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anims")
		class UAnimMontage* CombatMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* LeftEquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class AWeapon* RightEquippedWeapon;

	UFUNCTION()
	void SetEquippedWeapon(AWeapon* WeaponToSet, EEquippedWeapon Arm);
	AWeapon* GetEquippedWeapon(EEquippedWeapon Arm);

	void DebugEquip();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
		class UStaticMesh* DebugWeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		int MaxComboCount;

	void Defend();
	void DefendEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		bool bDefending;

	//check is the object has reach the requirement to outline for interactable action
	UFUNCTION()
		void OutlineCheck(class USphereComponent* CollisionVolume, int objectTypeIdx);

	//interact with outlined object(pickup)
	void Pickup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		bool bHasCombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		class ABadGuy* CombatTarget;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat")
		FVector CombatTargetLocation;

	FORCEINLINE void SetCombatTarget(ABadGuy* Enemy) { CombatTarget = Enemy; }
	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; }
	FORCEINLINE float GetHealth() { return Health; }


	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float AddValue);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		class ABetterPlayerController* BetterPlayerController;

	// Interation with Door and Camera
	UFUNCTION(Category = "Interact")
		void InteractStart(float TargetRotation, bool Boolean, AActor* Door);
	UFUNCTION(Category = "Interact")
		void InteractStartSwitch(bool Boolean, AActor* Switch);

	UFUNCTION(Category = "Interact")
		void CameraRotatable(bool Boolean, AActor* Rotator);

	UFUNCTION(Category = "Interact")
		AActor* GetLastRotator();

	UPROPERTY(EditAnywhere, Category = "Debug | Anims")
		float MontageBlendOutTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
		bool bWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
		EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float InterpSpeed;

	bool bInterpToEnemy;

	void SetInterpToEnemy(bool Interp);

	FRotator GetLookAtRotationYaw(FVector Target);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat")
	bool bIsRolling;

	UFUNCTION(BlueprintCallable)
	void NextCombo();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Combat")
	bool bComboTime;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Combat")
	class AShield* EquippedShield;

	void SetEquippedShield(AShield* ShieldToSet);
	AShield* GetEquippedShield();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hair")
	USkeletalMesh* SkeletalHair;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hair")
	USkeletalMesh* SkeletalHalfHair;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float HealthPercentage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float PreviousHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bDeath;

	UFUNCTION(BlueprintCallable)
	void DeathEnd();
private:
	UPROPERTY()
	UAnimInstance* AnimInstance;

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

	void Die();
};
