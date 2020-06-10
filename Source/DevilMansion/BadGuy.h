// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BadGuy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus :uint8
{
	EMS_Idle UMETA(DeplayName = "Idle"),
	EMS_MoveToTarget UMETA(DeplayName = "MoveToTarget"),
	EMS_Attacking UMETA(DeplayName = "Attacking"),
	EMS_Attacked UMETA(DeplayName = "Attacked"),
	EMS_Dying UMETA(DeplayName = "Dying"),
	EMS_MAX UMETA(DeplayName = "DefaultMax"),
};

UCLASS()
class DEVILMANSION_API ABadGuy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABadGuy();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EEnemyMovementStatus EnemyMovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* AgroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* CombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status);

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<class AObjectOutline> Outline;

	UPROPERTY()
	class AObjectOutline* OutlineRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bCanDropItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray< TSubclassOf<class AItem>> ItemList;

	virtual float ResetAttackTimer() { return 0.f; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Basic Function for an enemy character
	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class ABetterPlayer* Targetone);

	UFUNCTION(BlueprintCallable)
	EEnemyMovementStatus GetEnemyMovementStatus() { return EnemyMovementStatus; };

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION(BlueprintCallable)
	void Attack();

	UFUNCTION(BlueprintCallable)
	void SetMovementSpeed(float Speed);

	UFUNCTION(BlueprintCallable)
	bool IsAttacking() { return (EnemyMovementStatus == EEnemyMovementStatus::EMS_Attacking); };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bOverlappingCombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	ABetterPlayer* CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bInterpToPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	AActor* ParentActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	bool bIsDeath;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	float Deathfall;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	float DeathfallRate;

	UFUNCTION(Category = "Item")
	void DropItem();

	UFUNCTION(Category = "Debug")
	void Die();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float OldHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthDecrementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealthDelay;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float DelayCounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(Category = "AI")
	void SetInterpToPlayer(bool Interp);

	UFUNCTION(Category = "AI")
	FRotator GetLookAtRotationYaw(FVector Target);

	UFUNCTION(Category = "AI")
	void SetParentSpawner(AActor * source);

	UFUNCTION(Category = "AI")
	void NextAction();
private:
	void HealthDecrementSystem();
};
