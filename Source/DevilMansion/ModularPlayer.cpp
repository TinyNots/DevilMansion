// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AModularPlayer::AModularPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Face = CreateDefaultSubobject<USkeletalMeshComponent>("Face");
	Face->SetupAttachment(GetMesh());
	Hair = CreateDefaultSubobject<USkeletalMeshComponent>("Hair");
	Hair->SetupAttachment(GetMesh());
	Gloves = CreateDefaultSubobject<USkeletalMeshComponent>("Gloves");
	Gloves->SetupAttachment(GetMesh());
	Shoes = CreateDefaultSubobject<USkeletalMeshComponent>("Shoes");
	Shoes->SetupAttachment(GetMesh());
	HeadGears = CreateDefaultSubobject<USkeletalMeshComponent>("HeadGears");
	HeadGears->SetupAttachment(GetMesh());
	ShoulderPad = CreateDefaultSubobject<USkeletalMeshComponent>("ShoulderPad");
	ShoulderPad->SetupAttachment(GetMesh());
	Belt = CreateDefaultSubobject<USkeletalMeshComponent>("Belt");
	Belt->SetupAttachment(GetMesh());

	RightWeapon = CreateDefaultSubobject<UStaticMeshComponent>("RightWeapon");
	RightWeapon->SetupAttachment(GetMesh(),"RightWeaponShield");
	LeftWeapon = CreateDefaultSubobject<UStaticMeshComponent>("LeftWeapon");
	LeftWeapon->SetupAttachment(GetMesh(), "LeftWeaponShield");
	Shield = CreateDefaultSubobject<UStaticMeshComponent>("Shield");
	Shield->SetupAttachment(GetMesh(), "LeftWeaponShield");
	Backpack = CreateDefaultSubobject<UStaticMeshComponent>("Backpack");
	Backpack->SetupAttachment(GetMesh(), "Backpack");

	Health = 100.0f;
	Stamina = 100.0f;
	Mana = 100.0f;
	Armor = 1.0f;
}

// Called when the game starts or when spawned
void AModularPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AModularPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AModularPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}