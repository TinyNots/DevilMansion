// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterTrigger.h"
#include "ElevatorSwitch.h"
#include "Interactive.h"
#include "BetterPlayer.h"
#include "BadGuy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMonsterTrigger::AMonsterTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// trigger box that open the door
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(GetRootComponent());
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	TriggerBox->SetBoxExtent(FVector(62.f, 62.f, 64.f));
}

// Called when the game starts or when spawned
void AMonsterTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMonsterTrigger::TriggerBoxOnOverlapBegin);

	activated = false;

	SpawnedEnemyCount = 0;
}

// Called every frame
void AMonsterTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMonsterTrigger::TriggerBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && !activated)
	{
		ABetterPlayer* Main = Cast<ABetterPlayer>(OtherActor);
		if (Main)
		{
			for (auto spawns : Spawns)
			{
				// Look at the player who triggered this
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Main->GetActorLocation());
				FRotator LookAtRotationYaw(0.0f, LookAtRotation.Yaw, 0.0f);

				// Spawn
				ABadGuy* InstancedTarget = GetWorld()->SpawnActor<ABadGuy>(spawns.TargetActor, spawns.SpawnPosition, LookAtRotationYaw);
		
				// Chase player who triggered this
				if (spawns.ChasePlayer)
				{
					ABadGuy* Badguy = Cast<ABadGuy>(InstancedTarget);
					if (Badguy)
					{
						Badguy->MoveToTarget(Main);
						Badguy->SetParentSpawner(this);
						SpawnedEnemyCount++;
					}
				}
			}

			// PlaySound
			if (SpawnSound)
			{
				UGameplayStatics::PlaySound2D(this, SpawnSound);
				UE_LOG(LogTemp, Warning, TEXT("sound activate"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("sound = null"));
			}

			// Disable this trigger
			activated = true;
			TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void AMonsterTrigger::SpawnedEnemyDeath()
{
	SpawnedEnemyCount--;

	UE_LOG(LogTemp, Warning, TEXT("%d"), SpawnedEnemyCount);
	if (SpawnedEnemyCount <= 0 && ActivationObject)
	{
		AInteractive* Object = Cast<AInteractive>(ActivationObject);
		if (Object)
		{
			Object->UnlockDoor(false);
		}
		else
		{
			AElevatorSwitch* Switch = Cast<AElevatorSwitch>(ActivationObject);
			if (Switch)
			{
				Switch->ActivateSwitch();
			}
		}
		Destroy();
	}
}