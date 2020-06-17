// Fill out your copyright notice in the Description page of Project Settings.
#include "Interactive.h"
#include "BetterPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AInteractive::AInteractive()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//trigger box that close the door
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	//Make Trigger Ignore everything but reponse to pawn interaction only, this can save memory.
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	TriggerBox->SetBoxExtent(FVector(124.f, 124.f, 64.f));

	// trigger box that open the door
	TriggerBoxLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxLeft"));
	TriggerBoxLeft->SetupAttachment(GetRootComponent());
	TriggerBoxLeft->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBoxLeft->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBoxLeft->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBoxLeft->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	TriggerBoxLeft->SetBoxExtent(FVector(62.f, 62.f, 64.f));

	TriggerBoxRight = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxRight"));
	TriggerBoxRight->SetupAttachment(GetRootComponent());
	TriggerBoxRight->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBoxRight->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBoxRight->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBoxRight->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	TriggerBoxRight->SetBoxExtent(FVector(62.f, 62.f, 64.f));
	

	Door = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(GetRootComponent());

	RotateRate = 0.95f;
	//bRotateEnable = false;
}

// Called when the game starts or when spawned
void AInteractive::BeginPlay()
{
	Super::BeginPlay();

	TriggerBoxLeft->OnComponentBeginOverlap.AddDynamic(this, &AInteractive::TriggerBoxLeftOnOverlapBegin);
	TriggerBoxRight->OnComponentBeginOverlap.AddDynamic(this, &AInteractive::TriggerBoxRightOnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AInteractive::TriggerBoxOnOverlapEnd);

	Delta = 0.0f;
	CameraRotateRate = FMath::Max(FMath::Min(CameraRotateRate, 1.0f), 0.05f);
	this->SetActorTickEnabled(false);
}

// Called every frame
void AInteractive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Delta += DeltaTime;
	CameraDelta += CameraRotateRate;
	CameraDelta = FMath::Max(FMath::Min(CameraDelta, 1.001f), 0.00f);

	// Door Open/Close
	if (Door)
	{
		FTransform DoorTrans = Door->GetRelativeTransform();
		FQuat Rot = DoorTrans.GetRotation();
		FRotator CurrentTranslation = (FMath::RInterpTo(Rot.Rotator(), FRotator(0, TargetRotation, 0), Delta, RotateRate));
		Door->SetRelativeRotation(CurrentTranslation);

		// Camera Rotation
		if (CameraDelta != 1.001f && CameraOwner && bRotateEnable)
		{
			ABetterPlayer* Player = Cast<ABetterPlayer>(CameraOwner);
			if (Player)
			{
				Player->CameraBoom->SetRelativeRotation(FRotator(OriginalRotation.Pitch, FMath::Lerp(OriginalRotation.Yaw, CameraTargetRotation, CameraDelta), OriginalRotation.Roll));
			}
		}

		// If everything is done, stop looping this to save memory
		if (FMath::IsNearlyEqual(TargetRotation, Rot.Rotator().Yaw, 0.001f))
		{
			//Final Set
			Door->SetRelativeRotation(FRotator(0, TargetRotation, 0));
			if (bRotateEnable)
			{
				ABetterPlayer* Player = Cast<ABetterPlayer>(CameraOwner);
				if (Player)
				{
					Player->CameraBoom->SetRelativeRotation(FRotator(OriginalRotation.Pitch, CameraTargetRotation, OriginalRotation.Roll));
				}
			}


			// PlaySound
			if (DoorCloseSound && TargetRotation == 0.0f)
			{
				UGameplayStatics::PlaySound2D(this, DoorCloseSound);
			}

			this->SetActorTickEnabled(false);
		}
	}
}

void AInteractive::TriggerBoxLeftOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!(this->IsActorTickEnabled()) && TargetRotation == 0.f)
	{
		ABetterPlayer* Player = Cast<ABetterPlayer>(OtherActor);
		if (Player)
		{
			Player->InteractStart(-90.0f, true, this);
		}
	}
}

void AInteractive::TriggerBoxRightOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!(this->IsActorTickEnabled()) && TargetRotation == 0.f)
	{
		ABetterPlayer* Player = Cast<ABetterPlayer>(OtherActor);
		if (Player)
		{
			Player->InteractStart(90.0f, true, this);
		}
	}
}

void AInteractive::InteractDoorOpen(float Rotation)
{
	if (!Locked)
	{
		TargetRotation = Rotation;
		Delta = 0.0f;

		// PlaySound
		if (DoorOpenSound && this->IsActorTickEnabled() == false && Door->GetRelativeRotation().Yaw == 0.0f)
		{
			UGameplayStatics::PlaySound2D(this, DoorOpenSound, SoundVolume);
		}

		this->SetActorTickEnabled(true);
	}
}

void AInteractive::TriggerBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABetterPlayer* Player = Cast<ABetterPlayer>(OtherActor);
	CameraOwner = OtherActor;

	if (CameraOwner && Player)
	{
		// Camera Rotate
		if (TargetRotation == -90.0f)
		{
			CameraTargetRotation = CameraRotateLeft;
		}
		else if (TargetRotation == 90.0f)
		{
			CameraTargetRotation = CameraRotateRight;
		}
		OriginalRotation = Player->CameraBoom->GetRelativeRotation();


		// Door Rotate
		if (TargetRotation != 0.f)
		{
			TargetRotation = 0.0f;
		}

		// Reset
		Delta = 0.0f;
		CameraDelta = 0.0f;
		Player->InteractStart(0.f, false, nullptr);

		// PlaySound
		if (DoorOpenSound && this->IsActorTickEnabled() == false && PlaySoundDuringClose && Door->GetRelativeRotation().Yaw != 0.0f)
		{
			UGameplayStatics::PlaySound2D(this, DoorOpenSound, SoundVolume);
		}

		// Enable Loop
		this->SetActorTickEnabled(true);
	}
}

void AInteractive::UnlockDoor(bool lock)
{
	Locked = lock;
}