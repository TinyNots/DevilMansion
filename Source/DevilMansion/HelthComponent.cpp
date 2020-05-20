// Fill out your copyright notice in the Description page of Project Settings.


#include "HelthComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/KismetMathLibrary.h"

UHelthComponent::UHelthComponent()
{

}

void UHelthComponent::UpdateHealth()
{
	APlayerCameraManager *CamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	FVector CamLocation = CamManager->GetCameraLocation();
	FVector CamForward = CamManager->GetCameraRotation().Vector();

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetRelativeLocation(), CamLocation);
	SetRelativeRotation(FRotator(45.0f, NewRotation.Yaw, 0.0f));
}