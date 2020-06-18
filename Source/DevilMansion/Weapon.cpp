// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "BetterPlayer.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BadGuy.h"
#include "ObjectOutline.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetRootComponent());

	Damage = 100.0f;
	WeaponType = EWeaponType::EMS_NoWeapon;
	MaxCombo = 3;
	bCanBePickUp = true;
	bRotate = true;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	if (Outline)
	{
		OutlineRef->CollisionVolume->SetRelativeTransform(CollisionVolume->GetRelativeTransform());
	}

	/*if (Outline)
	{
		OutlineRef = GetWorld()->SpawnActor<AObjectOutline>(Outline, GetTransform());
		OutlineRef->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		OutlineRef->SetOwner(this);
		OutlineRef->VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogTemp, Warning, TEXT("Spawn Outline"));
	}*/
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if (OutlineRef)
	{
		if (OutlineRef->bOutlining)
		{
			VisualMesh->SetRenderCustomDepth(true);
		}
		else
		{
			VisualMesh->SetRenderCustomDepth(false);
		}
	}*/
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::PickUp(ABetterPlayer* Player)
{
	Equip(Player);
}

void AWeapon::Equip(ABetterPlayer* Char)
{
	if (Char && bCanBePickUp)
	{
		SetInstigator(Char->GetController());

		const USkeletalMeshSocket* HandSocket = Char->GetMesh()->GetSocketByName("RightWeaponShield");
		EEquippedWeapon CurrentSide = EEquippedWeapon::EMS_RightEquippedWeapon;
		if (WeaponType == EWeaponType::EMS_SingleTwohand)
		{
			HandSocket = Char->GetMesh()->GetSocketByName("LeftWeaponShield");
			CurrentSide = EEquippedWeapon::EMS_LeftEquippedWeapon;
		}

		AWeapon* OldWeaponLeft = Char->GetEquippedWeapon(EEquippedWeapon::EMS_LeftEquippedWeapon);
		AWeapon* OldWeaponRight = Char->GetEquippedWeapon(EEquippedWeapon::EMS_RightEquippedWeapon);

		if (WeaponType == EWeaponType::EMS_DoubleSword && OldWeaponRight)
		{
			if (OldWeaponRight->WeaponType == EWeaponType::EMS_DoubleSword)
			{
				HandSocket = Char->GetMesh()->GetSocketByName("LeftWeaponShield");
				CurrentSide = EEquippedWeapon::EMS_LeftEquippedWeapon;
			}
		}
		else
		{
			if (OldWeaponLeft)
			{
				Char->LeftEquippedWeapon = nullptr;
				OldWeaponLeft->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				OldWeaponLeft->SetActorTransform(this->GetTransform());
				OldWeaponLeft->WeaponInstigator = nullptr;
				OldWeaponLeft->bCanBePickUp = true;
				OldWeaponLeft->bRotate = true;

				OldWeaponLeft->OutlineRef->bEnableOutline = true;
			}

			if (OldWeaponRight)
			{
				Char->RightEquippedWeapon = nullptr;
				OldWeaponRight->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				OldWeaponRight->SetActorTransform(this->GetTransform());
				OldWeaponRight->WeaponInstigator = nullptr;
				OldWeaponRight->bCanBePickUp = true;
				OldWeaponRight->bRotate = true;

				OldWeaponRight->OutlineRef->bEnableOutline = true;
			}
		}

		if (HandSocket)
		{
			bRotate = false;
			HandSocket->AttachActor(this, Char->GetMesh());
			VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Char->SetEquippedWeapon(this, CurrentSide);
		}

		if (OutlineRef)
		{
			OutlineRef->bEnableOutline = false;
			bCanBePickUp = false;
		}
	}
}

void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABadGuy* Enemy = Cast<ABadGuy>(OtherActor);
		if (Enemy)
		{
			/*if (Enemy->HitParticles)
			{

			}*/
			if (Enemy->GetHitSound)
			{
				UGameplayStatics::PlaySound2D(this, Enemy->GetHitSound);
			}
		}
		if (DamageTypeClass)
		{
			UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstigator, this->GetAttachParentActor(), DamageTypeClass);
		}
		
	}
}

void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AWeapon::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AWeapon::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}