// Fill out your copyright notice in the Description page of Project Settings.


#include "Orc.h"
#include "Components/SphereComponent.h"

// Sets default values
AOrc::AOrc()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere->InitSphereRadius(1000.f);
	CombatSphere->InitSphereRadius(75.f);
	AttackTimerMax = 3.f;
	AttackRange = 250.f;
	AttackDamage = 30.0f;
}

// Called when the game starts or when spawned
void AOrc::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AOrc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void AOrc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AOrc::ResetAttackTimer()
{
	return AttackTimerMax;
}

