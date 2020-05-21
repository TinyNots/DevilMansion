// Fill out your copyright notice in the Description page of Project Settings.


#include "BadGuy.h"
#include "Components/SphereComponent.h"
#include "BetterPlayer.h"
#include "AIController.h"
#include "ObjectOutline.h"
#include "Item.h"

// Sets default values
ABadGuy::ABadGuy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());

	bCanDropItem = true;

	//OutlineMaterialIndex = 1;

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

	//if (Outline)
	//{
	//	OutlineRef = GetWorld()->SpawnActor<AObjectOutline>(Outline, GetTransform());
	//	OutlineRef->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//	OutlineRef->SetOwner(this);
	//	if (AlternateMeshAsset)
	//	{
	//		OutlineRef->SkeletalMesh->SetSkeletalMesh(AlternateMeshAsset);
	//	}
	//	OutlineRef->SkeletalMesh->SetMaterial(0, OutlineRef->SkeletalMesh->GetMaterial(OutlineMaterialIndex));
	//	UE_LOG(LogTemp, Warning, TEXT("Spawn Outline"));
	//}
}

// Called every frame
void ABadGuy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DropItem();
}

// Called to bind functionality to input
void ABadGuy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Die", EInputEvent::IE_Pressed, this, &ABadGuy::Die);


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
			CombatTarget = Main;
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
			bOverlappingCombatSphere = true;
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

void ABadGuy::DropItem()
{
	if (bCanDropItem )
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
				maxRoll += 100;
			}
		}
		int roll = FMath::RandRange(1, maxRoll);
		int32 rollOutCome = 0;
		for (int i = 0 ; i < ItemList.Num();i++)
		{
			int tmp = 0;
			if (ItemList[i])
			{
				tmp += ItemList[i].GetDefaultObject()->ItemDropRate;
			}
			else
			{
				tmp += 100;
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