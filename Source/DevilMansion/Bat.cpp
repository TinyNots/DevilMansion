// Fill out your copyright notice in the Description page of Project Settings.

#include "Bat.h"
#include "Components/SphereComponent.h"
#include "BetterPlayer.h"
#include "AIController.h"

// Sets default values
ABat::ABat()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere->InitSphereRadius(1000.f);
	CombatSphere->InitSphereRadius(75.f);
	AttackTimerMax = 3.0f;
}

// Called when the game starts or when spawned
void ABat::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ABat::ResetAttackTimer()
{
	return AttackTimerMax;
}
