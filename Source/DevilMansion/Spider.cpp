// Fill out your copyright notice in the Description page of Project Settings.

#include "Spider.h"
#include "Components/SphereComponent.h"
#include "BetterPlayer.h"
#include "AIController.h"

// Sets default values
ASpider::ASpider()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere->InitSphereRadius(1000.f);
	CombatSphere->InitSphereRadius(40.f);
	AttackTimerMax = 3.0f;
}

// Called when the game starts or when spawned
void ASpider::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ASpider::ResetAttackTimer()
{
	return AttackTimerMax;
}
