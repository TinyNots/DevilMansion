// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraRotator.h"
#include "Components/BoxComponent.h"
#include "BetterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACameraRotator::ACameraRotator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// trigger box that open the door
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxLeft"));
	TriggerBox->SetupAttachment(GetRootComponent());
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	TriggerBox->SetBoxExtent(FVector(62.f, 62.f, 64.f));
}

// Called when the game starts or when spawned
void ACameraRotator::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACameraRotator::TriggerBoxOnOverlapBegin);
}

// Called every frame
void ACameraRotator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update
	CameraDelta += CameraRotateRate;
	CameraDelta = FMath::Max(FMath::Min(CameraDelta, 1.001f), 0.00f);

	// Camera Rotation
	if (CameraDelta != 1.001f && CameraOwner)
	{
		ABetterPlayer* Player = Cast<ABetterPlayer>(CameraOwner);
		Player->CameraBoom->SetRelativeRotation(FRotator(OriginalRotation.Pitch, FMath::Lerp(OriginalRotation.Yaw, TargetAngle, CameraDelta), OriginalRotation.Roll));
		Player->CameraBoom->TargetArmLength = FMath::Lerp(OriginalCameraHeight, CameraHeight, CameraDelta);
	}

	// If everything is done, stop looping this to save memory
	if (CameraDelta == 1.0f)
	{
		// Final set
		ABetterPlayer* Player = Cast<ABetterPlayer>(CameraOwner);
		Player->CameraBoom->SetRelativeRotation(FRotator(OriginalRotation.Pitch, TargetAngle, OriginalRotation.Roll));
		Player->CameraBoom->TargetArmLength = CameraHeight;

		this->SetActorTickEnabled(false);
	}
}

void ACameraRotator::TriggerBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABetterPlayer* Player = Cast<ABetterPlayer>(OtherActor);
	CameraOwner = OtherActor;

	// Camera Rotate
	OriginalRotation = Player->CameraBoom->GetRelativeRotation();

	// Camera Height
	OriginalCameraHeight = Player->CameraBoom->TargetArmLength;

	// Reset
	CameraDelta = 0.0f;

	// Check if the rotator has been used
	ACameraRotator* LastRotator = Cast<ACameraRotator>(Player->GetLastRotator());
	if (LastRotator != nullptr)
	{
		//stop it
		LastRotator->RotatorOverlap();
	}

	// Check if the rotation isn't needed at all
	UE_LOG(LogTemp, Warning, TEXT("================================Rotator Activated=============================================="));
	UE_LOG(LogTemp, Warning, TEXT("OriginalAngle = %f, TargetAngle = %f"), OriginalRotation.Yaw, TargetAngle);
	UE_LOG(LogTemp, Warning, TEXT("OriginalHeight = %f, TargetHeight = %f"), OriginalCameraHeight, CameraHeight);
	if (OriginalRotation.Yaw == TargetAngle 
		&& OriginalCameraHeight == CameraHeight)
	{

		UE_LOG(LogTemp, Warning, TEXT("================================Not Working=============================================="));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("================================Working=============================================="));

	// Start Loop
	this->SetActorTickEnabled(true);
}

void ACameraRotator::RotatorOverlap()
{
	this->SetActorTickEnabled(false);
}