// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "BetterPlayer.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BadGuy.h"
#include "ObjectOutline.h"


AWeapon::AWeapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetRootComponent());

	Damage = 100.0f;
	WeaponType = EWeaponType::EMS_NoWeapon;
	MaxCombo = 3;
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
	if (OutlineRef)
	{
		if (OutlineRef->bOutlining)
		{
			VisualMesh->SetRenderCustomDepth(true);
		}
		else
		{
			VisualMesh->SetRenderCustomDepth(false);
		}
	}
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	/*if (OtherActor)
	{
		ABetterPlayer* Player = Cast<ABetterPlayer>(OtherActor);
		if (Player)
		{
			Equip(Player);
		}
	}*/
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::Equip(ABetterPlayer* Char)
{
	if (Char)
	{
		SetInstigator(Char->GetController());

		if (Char->GetEquippedWeapon(EEquippedWeapon::EMS_LeftEquippedWeapon) == nullptr && 
			Char->GetEquippedWeapon(EEquippedWeapon::EMS_RightEquippedWeapon) == nullptr)
		{
			const USkeletalMeshSocket* HandSocket = Char->GetMesh()->GetSocketByName("RightWeaponShield");
			EEquippedWeapon CurrentSide = EEquippedWeapon::EMS_RightEquippedWeapon;
			if (WeaponType == EWeaponType::EMS_SingleTwohand)
			{
				HandSocket = Char->GetMesh()->GetSocketByName("LeftWeaponShield");
				CurrentSide = EEquippedWeapon::EMS_LeftEquippedWeapon;
			}

			if (HandSocket)
			{
				bRotate = false;
				HandSocket->AttachActor(this, Char->GetMesh());
				VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Char->SetEquippedWeapon(this, CurrentSide);
			}
		}
		else if (Char->GetEquippedWeapon(EEquippedWeapon::EMS_LeftEquippedWeapon) != nullptr || 
				 Char->GetEquippedWeapon(EEquippedWeapon::EMS_RightEquippedWeapon) != nullptr)
		{
			if (WeaponType == EWeaponType::EMS_DoubleSword)
			{
				const USkeletalMeshSocket* HandSocket = Char->GetMesh()->GetSocketByName("LeftWeaponShield");
				if (HandSocket)
				{
					bRotate = false;
					HandSocket->AttachActor(this, Char->GetMesh());
					VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					Char->SetEquippedWeapon(this, EEquippedWeapon::EMS_LeftEquippedWeapon);
				}
			}
			if (OutlineRef)
				OutlineRef->bEnableOutline = false;
			{
			}
		}


		//if (Char->GetEquippedWeapon() == nullptr)
		//{
		//	SetInstigator(Char->GetController());
		//	/*SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		//	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		//	SkeletalMesh->SetSimulatePhysics(false);*/

		//	const USkeletalMeshSocket* HandSocket;
		//	if (WeaponType == EWeaponType::EMS_SingleTwohand)
		//	{
		//		HandSocket = Char->GetMesh()->GetSocketByName("LeftWeaponShield");
		//	}
		//	else
		//	{
		//		HandSocket = Char->GetMesh()->GetSocketByName("RightWeaponShield");
		//	}

		//	if (HandSocket)
		//	{
		//		HandSocket->AttachActor(this, Char->GetMesh());
		//		VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//		bRotate = false;
		//		Char->SetEquippedWeapon(this,EEquippedWeapon::EMS_LeftEquippedWeapon);
		//	}
		//}
		//else if(Char->GetEquippedWeapon(EEquippedWeapon::EMS_LeftEquippedWeapon) != this || Char->GetEquippedWeapon(EEquippedWeapon::EMS_RightEquippedWeapon) != this)
		//{
		//	SetInstigator(Char->GetController());
		//	const USkeletalMeshSocket* HandSocket;
		//	if (WeaponType == EWeaponType::EMS_SingleTwohand)
		//	{
		//		HandSocket = Char->GetMesh()->GetSocketByName("LeftWeaponShield");
		//	}
		//	else
		//	{
		//		HandSocket = Char->GetMesh()->GetSocketByName("RightWeaponShield");
		//	}

		//	AWeapon* OldWeapon = Char->GetEquippedWeapon();
		//	OldWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//	OldWeapon->SetActorTransform(this->GetTransform());

		//	if (HandSocket)
		//	{
		//		HandSocket->AttachActor(this, Char->GetMesh());
		//		VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//		bRotate = false;
		//		Char->SetEquippedWeapon(this);
		//	}
		//}
		
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