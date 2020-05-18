// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AModularPlayer::AModularPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void AModularPlayer::AttachMeshToSocket(USkeletalMeshComponent* SkeletalMesh, FName SocketName)
{
}

void AModularPlayer::AttachMeshToSocket(UStaticMeshComponent* StaticMesh, FName SocketName)
{

}