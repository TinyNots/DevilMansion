// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectOutline.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(VisualMesh);

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	CollisionVolume->SetupAttachment(GetRootComponent());

	IdleParticles = CreateDefaultSubobject<UParticleSystemComponent>("IdleParticles");
	IdleParticles->SetupAttachment(GetRootComponent());

	bRotate = false;
	RotationRate = 45.f;
	bCanOutline = true;
	OutlineMaterialIndex = 1;
	ItemDropRate = 100;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
	if (bCanOutline && Outline)
	{
		OutlineRef = GetWorld()->SpawnActor<AObjectOutline>(Outline);


		OutlineRef->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		OutlineRef->SetOwner(this);
		OutlineRef->SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		OutlineRef->VisualMesh->SetStaticMesh(this->VisualMesh->GetStaticMesh());
		OutlineRef->VisualMesh->SetMaterial(0, OutlineRef->VisualMesh->GetMaterial(OutlineMaterialIndex));
		OutlineRef->VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OutlineRef->bCanPickup = true;
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotate)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(Rotation);
	}

}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Item::OnOverlapBegin()"));
	//if (OverlapParticles)
	//{
	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
	//}

	//if (OverlapSound)
	//{
	//	UGameplayStatics::PlaySound2D(this, OverlapSound);
	//}

	//if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	//{
	//	/*if (OutlineRef->Destroy())
	//	{
	//		Destroy();
	//	}*/
	//	UE_LOG(LogTemp, Warning, TEXT("Destroy Item"));
	//}
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Item::OnOverlapEnd()"));
}

void AItem::PickupEffect()
{
	if (OverlapParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
	}

	if (OverlapSound)
	{
		UGameplayStatics::PlaySound2D(this, OverlapSound);
	}

	if (OutlineRef->Destroy())
	{
		Destroy();
	}
	UE_LOG(LogTemp, Warning, TEXT("Destroy Item"));
}

void AItem::PickUp(ABetterPlayer* Player)
{
	if (OutlineRef->bOutlining)
	{
		if (OutlineRef->bCanPickup)
		{
			PickupEffect();
		}
	}
}