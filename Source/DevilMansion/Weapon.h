// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	EMS_NoWeapon UMETA(DeplayName = "NoWeapon"),
	EMS_SwordShield UMETA(DeplayName = "SwordShield"),
	EMS_DoubleSword UMETA(DeplayName = "DoubleSword"),
	EMS_SingleTwohand UMETA(DeplayName = "SingleTwohand"),
	EMS_Bow UMETA(DeplayName = "Bow"),
	EMS_MagicWand UMETA(DeplayName = "MagicWand"),
	EMS_MAX UMETA(DeplayName = "DefaultMax")
};

UCLASS()
class DEVILMANSION_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Combat")
	class UBoxComponent* CombatCollision;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item | Combat")
	float Damage;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
public:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void Equip(class ABetterPlayer* Char);

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();

	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AController* WeaponInstigator;

	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	int MaxCombo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	bool bCanBePickUp;
};