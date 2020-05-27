// Fill out your copyright notice in the Description page of Project Settings.


#include "Torch.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "BetterPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "FogOfWarManager.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ATorch::ATorch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;

	bLightUp = false;
	bIsRegisterToFow = false;
}

// Called when the game starts or when spawned
void ATorch::BeginPlay()
{
	Super::BeginPlay();
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ATorch::OnOverlapBegin);
}

// Called every frame
void ATorch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AFogOfWarManager* FOWMng = Cast< AFogOfWarManager>(UGameplayStatics::GetActorOfClass(GetWorld(), FowManager));
	
	if (FOWMng && bLightUp && !bIsRegisterToFow)
	{
		FOWMng->RegisterFowActor(this, 1);
		bIsRegisterToFow = true;
	}
	
}

void ATorch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABetterPlayer* Main = Cast<ABetterPlayer>(OtherActor);
		if (Main && !bLightUp)
		{
			for (auto firePos:FirePosition)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticles, firePos, FRotator(0.f), true);
			bLightUp = true;
		}
	}
}

