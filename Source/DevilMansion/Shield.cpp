// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"
#include "BetterPlayer.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SkeletalMeshComponent.h"

AShield::AShield()
{
	BlockCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockCollision"));
	BlockCollision->SetupAttachment(GetRootComponent());
}

void AShield::BeginPlay()
{
	Super::BeginPlay();
}

void AShield::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor)
	{
		ABetterPlayer* Player = Cast<ABetterPlayer>(OtherActor);
		if (Player)
		{
			Equip(Player);
		}
	}
}

void AShield::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AShield::Equip(ABetterPlayer* Char)
{
	if (Char && Char->GetEquippedWeapon() != nullptr)
	{
		if (Char->GetEquippedWeapon()->WeaponType == EWeaponType::EMS_SwordShield &&
			Char->GetEquippedShield() == nullptr)
		{
			const USkeletalMeshSocket* LeftWeaponShield = Char->GetMesh()->GetSocketByName("LeftWeaponShield");
			if (LeftWeaponShield)
			{
				LeftWeaponShield->AttachActor(this, Char->GetMesh());
				VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				bRotate = false;
				VisualMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FQuat(0.0f, 0.0f, 0.0f, 0.0f));
				Char->SetEquippedShield(this);
			}
		}
	}
}
