// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Armor.generated.h"

UENUM(BlueprintType)
enum class EArmorType :uint8
{
	EMS_Armor UMETA(DeplayName = "Armor"),
	EMS_Gloves UMETA(DeplayName = "Gloves"),
	EMS_Belt UMETA(DeplayName = "Belt"),
	EMS_Shoes UMETA(DeplayName = "Shoes"),
	EMS_HeadGears UMETA(DeplayName = "HeadGears"),
	EMS_HeadGearsHalf UMETA(DeplayName = "HeadGearsHalf"),
	EMS_HeadGearsNone UMETA(DeplayName = "HeadGearsNone"),
	EMS_ShoulderPad UMETA(DeplayName = "ShoulderPad"),
	EMS_MAX UMETA(DeplayName = "DefaultMax")
};

UCLASS()
class DEVILMANSION_API AArmor : public AItem
{
	GENERATED_BODY()
public:
	AArmor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Armor")
	class USkeletalMeshComponent* ArmorMeshComponent;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	virtual void PickUp(class ABetterPlayer* Player) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Armor")
	class USkeletalMesh* ArmorSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Armor")
	int DefensePoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Armor")
	EArmorType ArmorType;
};
