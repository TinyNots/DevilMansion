// Fill out your copyright notice in the Description page of Project Settings.


#include "DuplicateWithOutline.h"

// Sets default values
ADuplicateWithOutline::ADuplicateWithOutline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	VisualMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ADuplicateWithOutline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADuplicateWithOutline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Item::OnOverlapBegin()"));
	if (OverlapParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
	}

	if (OverlapSound)
	{
		UGameplayStatics::PlaySound2D(this, OverlapSound);
	}

	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Destroy Item"));
	}
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Item::OnOverlapEnd()"));
}

