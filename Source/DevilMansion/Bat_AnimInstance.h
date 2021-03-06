// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Bat_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DEVILMANSION_API UBat_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void AttackedEnd();

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void AttackingEnd();

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void DamageFrame();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class ABadGuy* ThisEnemy;
};
