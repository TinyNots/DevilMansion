// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Weapon.h"
#include "Checkpoint.generated.h"


USTRUCT(BlueprintType)
struct FSaveInfo
{
	GENERATED_BODY()

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector PlayerLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator PlayerRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<bool> UnfoggedData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FogOfWar)
	TMap<FString, bool> LightedUpTorch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FogOfWar)
	TMap<FString, bool> EnemyDeathInfo;

	FSaveInfo() {};
};
/**
 * 
 */
UCLASS()
class DEVILMANSION_API UCheckpoint : public USaveGame
{
	GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere, Category = SaveInfo)
        FString SaveSlotName;

    UPROPERTY(VisibleAnywhere, Category = SaveInfo)
        uint32 UserIndex;

	UPROPERTY(VisibleAnywhere, Category = SaveInfo)
	FSaveInfo SaveInfo;

    UCheckpoint();

};
