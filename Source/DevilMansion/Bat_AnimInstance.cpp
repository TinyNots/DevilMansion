// Fill out your copyright notice in the Description page of Project Settings.


#include "Bat_AnimInstance.h"
#include "BadGuy.h"
#include "Bat.h"
#include "SkeletonBoy.h"

void UBat_AnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			ThisEnemy = Cast<ABadGuy>(Pawn);
		}
	}
}

void UBat_AnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			ThisEnemy = Cast<ABadGuy>(Pawn);
		}
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();
	}
}