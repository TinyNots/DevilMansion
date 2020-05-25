// Fill out your copyright notice in the Description page of Project Settings.
#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "ObjectOutline.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	RootComponent = VisualMesh;
	VisualMesh->SetSimulatePhysics(true);

	OutlineMaterialIndex = 1;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (Outline)
	{
		OutlineRef = GetWorld()->SpawnActor<AObjectOutline>(Outline, GetTransform());
		OutlineRef->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		OutlineRef->SetOwner(this);
		if (AlternateMeshAsset)
		{
			OutlineRef->SkeletalMesh->SetSkeletalMesh(AlternateMeshAsset);
		}
		OutlineRef->SkeletalMesh->SetMaterial(0, OutlineRef->VisualMesh->GetMaterial(OutlineMaterialIndex));
		UE_LOG(LogTemp, Warning, TEXT("Spawn Outline"));
	}
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

