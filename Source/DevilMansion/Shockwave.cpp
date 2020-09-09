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

	WaveSpeed = 0.1f;
	MaxRadius = 100.0f;
	SphereRadiusSize = 1.0f;
	OriginalRaidusSize = SphereRadiusSize;
	Timer = 0.0f;
}

// Called when the game starts or when spawned
void AShockwave::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShockwave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Timer > 1.0f)
	{
		Destroy();
	}

	Timer += WaveSpeed * DeltaTime;
	float NewSphereRadiusSize = FMath::InterpCircularIn(OriginalRaidusSize, MaxRadius, Timer);
	StaticMeshComponent->SetWorldScale3D(FVector(NewSphereRadiusSize));
}
