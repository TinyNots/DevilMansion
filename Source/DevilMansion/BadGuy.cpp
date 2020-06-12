// Fill out your copyright notice in the Description page of Project Settings.


#include "BadGuy.h"
#include "MonsterTrigger.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BetterPlayer.h"
#include "AIController.h"
#include "ObjectOutline.h"
#include "Item.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "HealthSystem.h"
#include "BetterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "HealthWidget.h"


// Sets default values
ABadGuy::ABadGuy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//AIControllerClass = AAIController::StaticClass();

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());

	CombatSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

	bCanDropItem = true;
	DeathfallRate = 0.2f;
	Deathfall = 0.0f;

	//default
	// Health setting init
	MaxHealth = 100.0f;
	InterpSpeed = 15.0f;
	Health = MaxHealth;
	OldHealth = Health;
	HealthDecrementSpeed = 0.75f;
	HealthDelay = 1.0f;
	DelayCounter = 0.0f;
}

void ABadGuy::SetEnemyMovementStatus(EEnemyMovementStatus Status)
{
	if (EnemyMovementStatus == EEnemyMovementStatus::EMS_Dying) return;
	EnemyMovementStatus = Status;
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

	if (GetMesh())
	{
		GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	}

	// initialization
	bOverlappingCombatSphere = false;
	bIsDeath = false;
	Health = MaxHealth;

	if (Outline)
	{
		OutlineRef = GetWorld()->SpawnActor<AObjectOutline>(Outline, GetTransform());
		OutlineRef->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		OutlineRef->SetOwner(this);
		OutlineRef->CollisionVolume->SetCollisionObjectType(ECC_GameTraceChannel1);
	}
}

// Called every frame
void ABadGuy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DropItem();
	if (AttackTimer <= 0.0f)
	{
		NextAction();
	}
	else
	{
		AttackTimer -= DeltaTime;
	}

	if (OutlineRef)
	{
		if (OutlineRef->bOutlining)
		{
			GetMesh()->SetRenderCustomDepth(true);
		}
		else
		{
			GetMesh()->SetRenderCustomDepth(false);

		}
	}

	if (bInterpToPlayer && CombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);
	}

	if (bIsDeath)
	{
		FVector originalPos = this->GetTargetLocation();
		this->SetActorRelativeLocation(FVector(originalPos.X, originalPos.Y, originalPos.Z - DeathfallRate));
		Deathfall += DeathfallRate;
		if (Deathfall >= 100.0f)
		{
			this->SetActorTickEnabled(false);
		}
	}

	HealthDecrementSystem();
}

// Called to bind functionality to input
void ABadGuy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Die", EInputEvent::IE_Pressed, this, &ABadGuy::Die);
}

void ABadGuy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)	//check nullptr
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
			if (Main->CombatTarget == this)
			{
				Main->SetHasCombatTarget(false);
				Main->SetCombatTarget(nullptr);
				if (Main->BetterPlayerController)
				{
					Main->BetterPlayerController->RemoveEnemyHealthBar();
				}
			}

			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			CombatTarget = nullptr;
			if (AIController)
			{
				AIController->StopMovement();
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
			// Set player combat traget
			if (!Main->bHasCombatTarget)
			{
				Main->SetCombatTarget(this);
				Main->SetHasCombatTarget(true);
				if (Main->BetterPlayerController)
				{
					Main->BetterPlayerController->DisplayEnemyHealthBar();
				}
			}

			CombatTarget = Main;
			Attack();
		}
	}
}

void ABadGuy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == CombatTarget)
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
	if (bIsDeath) //condition
	{
		return;
	}

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

void ABadGuy::Death()
{
	// Check if it's already dead
	if (bIsDeath)
	{
		return;
	}

	// Set Status
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dying);

	// Stop Action
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (AIController)
	{
		AIController->StopMovement();
	}

	// Tell the spawner its death
	AMonsterTrigger* trigger = Cast<AMonsterTrigger>(ParentActor);
	if (trigger)
	{
		trigger->SpawnedEnemyDeath();
	}

	this->SetActorEnableCollision(false);
	// CombatTarget is player class
	if(CombatTarget && CombatTarget->CombatTarget == this)
	{
		CombatTarget->SetHasCombatTarget(false);
		CombatTarget->SetCombatTarget(nullptr);
		if (CombatTarget->BetterPlayerController)
		{
			CombatTarget->BetterPlayerController->RemoveEnemyHealthBar();
		}
	}

	bIsDeath = true;
}

void ABadGuy::Attack()
{
	if (CombatTarget && AttackTimer <= 0 && EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacked)
	{
		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		bOverlappingCombatSphere = true;

		AttackTimer = ResetAttackTimer();
		
		if (DamageTypeClass)
		{
			UGameplayStatics::ApplyDamage(CombatTarget, -10.0f, AIController, this, DamageTypeClass);
		}

		if (AIController)
		{
			AIController->StopMovement();
		}
	}
}

void ABadGuy::SetMovementSpeed(float Speed)
{
	MovementSpeed = Speed;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void ABadGuy::DropItem()
{
	if (bCanDropItem && ItemList.Num() > 0)
	{
		int maxRoll = 0;
		for (auto item : ItemList)
		{
			if (item)
			{
				maxRoll += item.GetDefaultObject()->ItemDropRate;
			}
			else
			{
				maxRoll += NOITEM_DROP_RATE;
			}
		}
		int roll = FMath::RandRange(1, maxRoll);
		int32 rollOutCome = 0;
		int tmp = 0;
		for (int i = 0 ; i < ItemList.Num();i++)
		{
			
			if (ItemList[i])
			{
				tmp += ItemList[i].GetDefaultObject()->ItemDropRate;
			}
			else
			{
				tmp += NOITEM_DROP_RATE;
			}
			if (tmp >= roll)
			{
				rollOutCome = i;
				break;
			}
		}
		if (ItemList[rollOutCome])
		{
			GetWorld()->SpawnActor<AItem>(ItemList[rollOutCome], GetTransform());
		}

		bCanDropItem = false;
	}
}

void ABadGuy::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("Die"));
	Destroy();
}

float ABadGuy::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	// Check if it's already dead
	if (bIsDeath)
	{
		return 0.0f;
	}

	Health -= DamageAmount;
	if (Health <= 0.0f)
	{
		Death();
	}
	else
	{
		// animation
		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacked);
		// stop moving for a sec
		if (AIController)
		{
			AIController->StopMovement();
		}
		// face to player
		CombatTarget = Cast<ABetterPlayer>(DamageCauser);
		if (CombatTarget)
		{
			SetActorRotation(GetLookAtRotationYaw(CombatTarget->GetActorLocation()));
		}
	}

	return DamageAmount;
}

void ABadGuy::SetInterpToPlayer(bool Interp)
{
	bInterpToPlayer = Interp;
}

FRotator ABadGuy::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.0f, LookAtRotation.Yaw, 0.0f);
	return LookAtRotationYaw;
}

void ABadGuy::NextAction()
{
	if (CombatTarget)
	{
		MoveToTarget(CombatTarget);
		if (FVector::Dist(this->GetActorLocation(), CombatTarget->GetActorLocation()) < (CombatSphere->GetScaledSphereRadius() * 2))
		{
			ABetterPlayer* Main = Cast<ABetterPlayer>(CombatTarget);
			if (Main)
			{
				CombatTarget = Main;
				Attack();
			}
		}
	}
}

void ABadGuy::HealthDecrementSystem()
{
	if (OldHealth != Health)
	{
		if (DelayCounter < HealthDelay * 60.0f)
		{
			DelayCounter++;
		}
		else
		{
			OldHealth -= 0.5f;
			if (OldHealth <= Health)
			{
				OldHealth = Health;
				DelayCounter = 0.0f;
			}
		}
	}
}

void ABadGuy::SetParentSpawner(AActor* source)
{
	ParentActor = source;
}