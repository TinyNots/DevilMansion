// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectOutline.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "BetterPlayer.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Item.h"


// Sets default values
AObjectOutline::AObjectOutline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	VisualMesh->SetupAttachment(GetRootComponent());

	OutlineMaterial = CreateDefaultSubobject<UMaterial>(TEXT("OutlineMaterial"));

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(GetRootComponent());

	bCanPickup = false;
}

// Called when the game starts or when spawned
void AObjectOutline::BeginPlay()
{
	Super::BeginPlay();
	bOutlining = false;
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AObjectOutline::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AObjectOutline::OnOverlapEnd);
}

// Called every frame
void AObjectOutline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bOutlining = false;
}

void AObjectOutline::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AObjectOutline::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ABetterPlayer* Main = Cast<ABetterPlayer>(OtherActor);
		if (Main)
		{
			bOutlining = false;
		}
	}
}

void AObjectOutline::Pickup()
{
	if (bOutlining)
	{
		if (bCanPickup)
		{
			AItem* item = Cast<AItem>(GetOwner());
			if (item)
			{
				UE_LOG(LogTemp, Warning, TEXT("Owner Acquired"));
				item->PickupEffect();
			}
		}
	}
}