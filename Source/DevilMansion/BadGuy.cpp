// Fill out your copyright notice in the Description page of Project Settings.


#include "BadGuy.h"
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



// Sets default values
ABadGuy::ABadGuy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());

	CombatSphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

	bCanDropItem = true;


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

	// initialization
	bOverlappingCombatSphere = false;
	bIsDeath = false;

	if (Outline)
	{
		OutlineRef = GetWorld()->SpawnActor<AObjectOutline>(Outline, GetTransform());
		OutlineRef->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		OutlineRef->SetOwner(this);
		OutlineRef->CollisionVolume->SetCollisionObjectType(ECC_GameTraceChannel1);
		UE_LOG(LogTemp, Warning, TEXT("Spawn Outline"));
	}
}

// Called every frame
void ABadGuy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DropItem();
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
			if (AIController)
			{
				AIController->StopMovement();
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
	this->Death();
}

void ABadGuy::MoveToTarget(ABetterPlayer* Targetone)
{
	if (bIsDeath) //condition
	{
		UE_LOG(LogTemp, Warning, TEXT("RETURN"));
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
	// Set Status
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dying);

	// Stop Action
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (AIController)
	{
		AIController->StopMovement();
		AIController->SetActorTickEnabled(false);
	}

	bIsDeath = true;
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
		UE_LOG(LogTemp, Warning, TEXT("Spawn Item"));

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
	Health -= DamageAmount;
	if (Health < 0.0f)
	{
		//Die
	}
	return DamageAmount;
}
