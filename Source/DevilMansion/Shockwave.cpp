// Fill out your copyright notice in the Description page of Project Settings.


#include "Shockwave.h"
#include "Components/SphereComponent.h"

// Sets default values
AShockwave::AShockwave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WaveCollision = CreateDefaultSubobject<USphereComponent>(TEXT("WaveCollision"));
	WaveCollision->SetupAttachment(GetRootComponent());
	WaveSpeed = 0.1f;
	WaveTime = 0.0f;
	WaveTimeMax = 1.0;
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

	if (WaveTime < WaveTimeMax * 60.0f)
	{
		float SphereRadius = WaveCollision->GetScaledSphereRadius() + WaveSpeed;
		WaveCollision->SetSphereRadius(SphereRadius);

		WaveTime++;
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