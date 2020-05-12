// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	RootComponent = VisualMesh;
	SpringArm->SetupAttachment(VisualMesh);
	Camera->SetupAttachment(SpringArm);

	VisualMesh->SetSimulatePhysics(true);
	MovementSpeed = 100000;
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("MoveX", this, &AMyPlayer::MoveX);
	InputComponent->BindAxis("MoveY", this, &AMyPlayer::MoveY);

}

void AMyPlayer::MoveX(float value)
{
	FVector2D mouse_pos;
	FVector char_pos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//GEngine->GameViewport->GetMousePosition(mouse_pos);

	//FVector diff = char_pos - FVector(mouse_pos.X, mouse_pos.Y, char_pos.Z);
	//FVector force = diff * MovementSpeed * value;
	FVector force = FVector(0,1,0) * value * MovementSpeed;
	VisualMesh->AddForce(force);
}

void AMyPlayer::MoveY(float value)
{
	FVector2D mouse_pos;
	FVector char_pos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	//GEngine->GameViewport->GetMousePosition(mouse_pos);

	//FVector diff = char_pos - FVector(mouse_pos.X, mouse_pos.Y, char_pos.Z);
	//FVector force = diff * MovementSpeed * value;
	FVector force = FVector(1, 0, 0) * value * MovementSpeed;
	VisualMesh->AddForce(force);
}


