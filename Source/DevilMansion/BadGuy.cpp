// Fill out your copyright notice in the Description page of Project Settings.


#include "BadGuy.h"
#include "Components/SphereComponent.h"
#include "BetterPlayer.h"
#include "AIController.h"
#include "HealthSystem.h"
#include "BetterPlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABadGuy::ABadGuy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());

	MaxHealth = 100.0f;
	Health = 100.0f;
}

// Called when the game starts or when spawned
void ABadGuy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &ABadGuy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &ABadGuy::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &ABadGuy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &ABadGuy::CombatSphereOnOverlapEnd);

	bOverlappingCombatSphere = false;
}

// Called every frame
void ABadGuy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABadGuy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABadGuy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABetterPlayer* Main = Cast<ABetterPlayer>(OtherActor);
		if (Main)
		{
			MoveToTarget(Main);
		}
	}
}

void ABadGuy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ABetterPlayer* Main = Cast<ABetterPlayer>(OtherActor);
		if (Main)
		{
			if (Main->BetterPlayerController)
			{
				Main->SetHasCombatTarget(false);
				Main->BetterPlayerController->RemoveEnemyHealthBar();
			}

			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			if (AIController)
			{
				AIController->StopMovement();
				CombatTarget = nullptr;
			}
		}
	}
}

void ABadGuy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABetterPlayer* Main = Cast<ABetterPlayer>(OtherActor);
		if (Main)
		{
			Main->SetCombatTarget(this);
			if (Main->BetterPlayerController)
			{
				Main->BetterPlayerController->DisplayEnemyHealthBar();
				Main->SetHasCombatTarget(true);
			}

			CombatTarget = Main;
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
			bOverlappingCombatSphere = true;

			if (DamageTypeClass)
			{
				UGameplayStatics::ApplyDamage(Main, 10.0f, AIController, this, DamageTypeClass);
			}
		}
	}
}

void ABadGuy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ABetterPlayer* Main = Cast<ABetterPlayer>(OtherActor);
		if (Main)
		{
			bOverlappingCombatSphere = false;
			if (EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking)
			{
				MoveToTarget(Main);
			}
		}
	}
}

void ABadGuy::MoveToTarget(ABetterPlayer* Targetone)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Targetone);
		MoveRequest.SetAcceptanceRadius(25.f);

		FNavPathSharedPtr NavPath;
		AIController->MoveTo(MoveRequest, &NavPath);
	}
}

float ABadGuy::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Health -= DamageAmount;
	if (Health < 0.0f)
	{
		//Die
	}
	return DamageAmount;
}
