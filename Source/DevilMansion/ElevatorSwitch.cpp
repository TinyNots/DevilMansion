// Fill out your copyright notice in the Description page of Project Settings.
#include "ElevatorSwitch.h"
#include "BetterPlayer.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BetterPlayerController.h"
#include "Components/TextBlock.h"

// Sets default values
AElevatorSwitch::AElevatorSwitch()
{

	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	TriggerSphere->SetupAttachment(GetRootComponent());
	InfoText = FText::FromString("Please wirte info here.");
}

// Called when the game starts or when spawned
void AElevatorSwitch::BeginPlay()
{
	Super::BeginPlay();
	
	bActivated = false;
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
		bActivated = true;

		// PlaySound
		if (LoopSound)
		{
			UGameplayStatics::PlaySound2D(this, LoopSound, SoundVolume);
		}

		// Start Looping
		this->SetActorTickEnabled(true);
	}
}

void AElevatorSwitch::TriggerSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)	//check nullptr
	{
		ABetterPlayer* Player = Cast<ABetterPlayer>(OtherActor);
		if (Player)
		{
			Player->InteractStartSwitch(true, this);

			// Fade in the info about this interactable object
			ABetterPlayerController* PlayerController = Player->BetterPlayerController;
			if (!PlayerController->bIsShowingInfo)
			{
				PlayerController->InfoTextBlock->SetText(InfoText);
				PlayerController->PlayWidgetAnimaiton("FadeIn");
				PlayerController->bIsShowingInfo = true;
			}
		}
	}
}

void AElevatorSwitch::TriggerSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)	//check nullptr
	{
		ABetterPlayer* Player = Cast<ABetterPlayer>(OtherActor);
		if (Player)
		{
			Player->InteractStartSwitch(false, nullptr);

			// Fade out the info about this interactable object
			ABetterPlayerController* PlayerController = Player->BetterPlayerController;
			if (PlayerController->bIsShowingInfo)
			{
				PlayerController->bIsShowingInfo = false;
				PlayerController->PlayWidgetAnimaiton("FadeOut");
			}
		}
	}
}