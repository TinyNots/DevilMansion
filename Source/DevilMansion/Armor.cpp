// Fill out your copyright notice in the Description page of Project Settings.


#include "Armor.h"
#include "BetterPlayer.h"
#include "Components/SkeletalMeshComponent.h"
#include "ObjectOutline.h"

AArmor::AArmor()
{
	ArmorMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("ArmorMesh");
	ArmorMeshComponent->SetupAttachment(GetRootComponent());

	ArmorType = EArmorType::EMS_Armor;
}

void AArmor::BeginPlay()
{
	Super::BeginPlay();
}

void AArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OutlineRef)
	{
		if (OutlineRef->bOutlining)
		{
			ArmorMeshComponent->SetRenderCustomDepth(true);
		}
		else
		{
			ArmorMeshComponent->SetRenderCustomDepth(false);

		}
	}
}

void AArmor::PickUp(ABetterPlayer* Player)
{
	if (ArmorSkeletalMesh)
	{
		if (ArmorType == EArmorType::EMS_Armor)
		{
			Player->GetMesh()->SetSkeletalMesh(ArmorSkeletalMesh);
		}
		else if(ArmorType == EArmorType::EMS_Belt)
		{
			Player->Belt->SetSkeletalMesh(ArmorSkeletalMesh);
		}
		else if (ArmorType == EArmorType::EMS_Gloves)
		{
			Player->Gloves->SetSkeletalMesh(ArmorSkeletalMesh);
		}
		else if (ArmorType == EArmorType::EMS_HeadGears)
		{
			Player->HeadGears->SetRelativeLocation(FVector(0.0f, 5.5f, 0.0f));
			Player->HeadGears->SetSkeletalMesh(ArmorSkeletalMesh);
			Player->Hair->SetSkeletalMesh(nullptr);
		}
		else if (ArmorType == EArmorType::EMS_HeadGearsHalf)
		{
			Player->HeadGears->SetRelativeLocation(FVector::ZeroVector);
			Player->HeadGears->SetSkeletalMesh(ArmorSkeletalMesh);
			Player->Hair->SetSkeletalMesh(Player->SkeletalHalfHair);
		}
		else if (ArmorType == EArmorType::EMS_HeadGearsNone)
		{
			Player->HeadGears->SetRelativeLocation(FVector::ZeroVector);
			Player->HeadGears->SetSkeletalMesh(ArmorSkeletalMesh);
		}
		else if (ArmorType == EArmorType::EMS_Shoes)
		{
			Player->Shoes->SetSkeletalMesh(ArmorSkeletalMesh);
		}
		else if (ArmorType == EArmorType::EMS_ShoulderPad)
		{
			Player->ShoulderPad->SetSkeletalMesh(ArmorSkeletalMesh);
		}

		Destroy();
	}
}