// Fill out your copyright notice in the Description page of Project Settings.
#include "ElevatorSwitch.h"
#include "BetterPlayer.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AElevatorSwitch::AElevatorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = VisualMesh;

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	TriggerSphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AElevatorSwitch::BeginPlay()
{
	Super::BeginPlay();
	

	// Initialization
	if (bEnableByDefault)
	{
		bEnabled = true;
	}
	else
	{
		bEnabled = false;
	}

	this->SetActorTickEnabled(false);

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &AElevatorSwitch::TriggerSphereOnOverlapBegin);
	TriggerSphere->OnComponentEndOverlap.AddDynamic(this, &AElevatorSwitch::TriggerSphereOnOverlapEnd);
}

// Called every frame
void AElevatorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the switch
	FTransform DoorTrans = VisualMesh->GetRelativeTransform();
	FQuat Rot = DoorTrans.GetRotation();
	VisualMesh->SetRelativeRotation(FRotator(0, Rot.Rotator().Yaw + 0.5f, 0));

	FVector originalPos = TargetObject->GetTargetLocation();
	// Move target object
	switch (SwitchType)
	{
	case ESwitchType::EST_UP:
		TargetObject->SetActorRelativeLocation(FVector(originalPos.X, originalPos.Y, originalPos.Z + MoveRate));
		break;
	case ESwitchType::EST_DOWN:
		TargetObject->SetActorRelativeLocation(FVector(originalPos.X, originalPos.Y, originalPos.Z - MoveRate));
		break;
	case ESwitchType::EST_LEFT:
		TargetObject->SetActorRelativeLocation(FVector(originalPos.X - MoveRate, originalPos.Y, originalPos.Z));
		break;
	case ESwitchType::EST_RIGHT:
		TargetObject->SetActorRelativeLocation(FVector(originalPos.X + MoveRate, originalPos.Y, originalPos.Z));
		break;
	default:
		//end the loop
		this->SetActorTickEnabled(false);
		break;
	}

	MovedDistance += MoveRate;
	if (MovedDistance >= TargetMoveDistance)
	{
		this->SetActorTickEnabled(false);
		bEnabled = true;

		//Set to reverse direction
		switch (SwitchType)
		{
		case ESwitchType::EST_UP:
			SwitchType = ESwitchType::EST_DOWN;
			break;
		case ESwitchType::EST_DOWN:
			SwitchType = ESwitchType::EST_UP;
			break;
		case ESwitchType::EST_LEFT:
			SwitchType = ESwitchType::EST_RIGHT;
			break;
		case ESwitchType::EST_RIGHT:
			SwitchType = ESwitchType::EST_LEFT;
			break;
		default:
			//end
			this->SetActorTickEnabled(false);
			break;
		}
	}
}

void AElevatorSwitch::ActivateSwitch()
{
	if (bEnabled && TargetObject != nullptr)
	{
		MovedDistance = 0.0f;

		// Disable
		bEnabled = false;

		// Start Looping
		this->SetActorTickEnabled(true);
	}
}

void AElevatorSwitch::TriggerSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)	//check nullptr
	{
		ABetterPlayer* Main = Cast<ABetterPlayer>(OtherActor);
		if (Main)
		{
			Main->InteractStartSwitch(true, this);
		}
	}
}

void AElevatorSwitch::TriggerSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)	//check nullptr
	{
		ABetterPlayer* Main = Cast<ABetterPlayer>(OtherActor);
		if (Main)
		{
			Main->InteractStartSwitch(false, nullptr);
		}
	}
}