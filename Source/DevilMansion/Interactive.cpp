// Fill out your copyright notice in the Description page of Project Settings.
#include "Interactive.h"
#include "BetterPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


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
}

// Called when the game starts or when spawned
void AInteractive::BeginPlay()
{
	Super::BeginPlay();

	TriggerBoxLeft->OnComponentBeginOverlap.AddDynamic(this, &AInteractive::TriggerBoxLeftOnOverlapBegin);
	TriggerBoxRight->OnComponentBeginOverlap.AddDynamic(this, &AInteractive::TriggerBoxRightOnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AInteractive::TriggerBoxOnOverlapEnd);

	Delta = 0.0f;
}

// Called every frame
void AInteractive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Delta += DeltaTime;

	// Door Open/Close
	FTransform DoorTrans = Door->GetRelativeTransform();
	FQuat Rot = DoorTrans.GetRotation();
	FRotator CurrentTranslation = (FMath::RInterpTo(Rot.Rotator(), FRotator(0, TargetRotation, 0), Delta, RotateRate));
	Door->SetRelativeRotation(CurrentTranslation);

	//// Camera Rotation
	ABetterPlayer* Player = Cast<ABetterPlayer>(CameraOwner);
	//Player->CameraBoom->SetRelativeRotation(FRotator(OriginalRotation.Pitch, FMath::Lerp(0.0f, CameraTargetRotation, Delta), OriginalRotation.Roll));

	//the door close or open completely. stop looping this to save memory
	if (FMath::IsNearlyEqual(TargetRotation, Rot.Rotator().Yaw, 0.001f))
	{
		this->SetActorTickEnabled(false);
	}
}

void AInteractive::TriggerBoxLeftOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!(this->IsActorTickEnabled()) && TargetRotation == 0.f)
	{
		ABetterPlayer* Player = Cast<ABetterPlayer>(OtherActor);
		Player->InteractStart(-90.0f, true, this);
	}
}

void AInteractive::TriggerBoxRightOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!(this->IsActorTickEnabled()) && TargetRotation == 0.f)
	{
		ABetterPlayer* Player = Cast<ABetterPlayer>(OtherActor);
		Player->InteractStart(90.0f, true, this);
	}
}

void AInteractive::InteractDoorOpen(float Rotation)
{
	TargetRotation = Rotation;
	Delta = 0.0f;
	this->SetActorTickEnabled(true);
}

void AInteractive::TriggerBoxOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABetterPlayer* Player = Cast<ABetterPlayer>(OtherActor);
	CameraOwner = OtherActor;

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
	Player->InteractStart(0.f, false, nullptr);

	// Enable Loop
	this->SetActorTickEnabled(true);
}