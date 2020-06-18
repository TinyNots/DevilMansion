// Fill out your copyright notice in the Description page of Project Settings.


#include "Shockwave.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "BadGuy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AShockwave::AShockwave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	WaveCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WaveCollision"));
	WaveCollision->SetupAttachment(GetRootComponent());

	WaveCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WaveCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	WaveCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	WaveSpeed = 0.1f;
	MaxRadius = 100.0f;
}

// Called when the game starts or when spawned
void AShockwave::BeginPlay()
{
	Super::BeginPlay();
	
	WaveCollision->OnComponentBeginOverlap.AddDynamic(this, &AShockwave::OnOverlapBegin);
	WaveCollision->OnComponentEndOverlap.AddDynamic(this, &AShockwave::OnOverlapEnd);
}

// Called every frame
void AShockwave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WaveCollision->GetScaledSphereRadius() < MaxRadius)
	{
		float SphereRadius = WaveCollision->GetScaledSphereRadius() + WaveSpeed;
		WaveCollision->SetSphereRadius(SphereRadius);
	}
	else
	{
		Destroy();
	}
}

void AShockwave::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

void AShockwave::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
}