// Fill out your copyright notice in the Description page of Project Settings.


#include "Skeletonboy.h"
#include "Components/SphereComponent.h"

// Sets default values
ASkeletonboy::ASkeletonboy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	AgroSphere->InitSphereRadius(1000.f);
	CombatSphere->InitSphereRadius(75.f);
}

// Called when the game starts or when spawned
void ASkeletonboy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkeletonboy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASkeletonboy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

