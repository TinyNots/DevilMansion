// Fill out your copyright notice in the Description page of Project Settings.


#include "Bat_AnimInstance.h"
#include "Bat.h"

void UBat_AnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Bat = Cast<ABat>(Pawn);
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
			FVector Speed = Pawn->GetVelocity();
			FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
			MovementSpeed = LateralSpeed.Size();

			Bat = Cast<ABat>(Pawn);
		}
	}
}