// Fill out your copyright notice in the Description page of Project Settings.


#include "BetterPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Weapon.h"
#include "ObjectOutline.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ShapeComponent.h"
#include "Item.h"

// Sets default values
ABetterPlayer::ABetterPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Create camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = true;
	CameraBoom->bInheritRoll = false;

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	EnemyCollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("EnemyCollisionVolume"));
	EnemyCollisionVolume->SetupAttachment(GetRootComponent());
	ItemCollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("ItemCollisionVolume"));
	ItemCollisionVolume->SetupAttachment(GetRootComponent());

	bAttacking = false;
	MaxComboCount = 3;
	ComboCount = 0;
}

// Called when the game starts or when spawned
void ABetterPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABetterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OutlineCheck(EnemyCollisionVolume);
	OutlineCheck(ItemCollisionVolume);

}

// Called to bind functionality to input
void ABetterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABetterPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveSide", this, &ABetterPlayer::MoveSide);

	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &ABetterPlayer::Attack);
	PlayerInputComponent->BindAction("DebugEquip", EInputEvent::IE_Pressed, this, &ABetterPlayer::DebugEquip);
	PlayerInputComponent->BindAction("Pickup", EInputEvent::IE_Pressed, this, &ABetterPlayer::Pickup);
}


void ABetterPlayer::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f && !bAttacking)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABetterPlayer::MoveSide(float Value)
{
	if (Controller != nullptr && Value != 0.0f && !bAttacking)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ABetterPlayer::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABetterPlayer::Attack()
{
	if (EquippedWeapon)
	{
		bAttacking = true;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage && ComboCount < MaxComboCount)
		{
			ComboCount++;
			AnimInstance->Montage_Play(CombatMontage);
			FName SectionName;
			switch (ComboCount)
			{
			case 1:
				SectionName = FName("Combo01");
				break;
			case 2:
				SectionName = FName("Combo02");
				break;
			case 3:
				SectionName = FName("Combo03");
				break;
			default:
				break;
			}
			AnimInstance->Montage_JumpToSection(SectionName, CombatMontage);
		}
	}
}

void ABetterPlayer::AttackEnd()
{
	bAttacking = false;
	ComboCount = 0;
}

void ABetterPlayer::DebugEquip()
{
	if (EquippedWeapon == nullptr)
	{
		AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>();
		if (Weapon)
		{
			const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName("RightWeaponShield");
			if (RightHandSocket)
			{
				RightHandSocket->AttachActor(Weapon, GetMesh());
			}

			Weapon->SetActorRelativeLocation(FVector(6.0f, 0, 0));
			Weapon->CollisionVolume->SetSphereRadius(0.0f);
			Weapon->VisualMesh->SetStaticMesh(DebugWeaponMesh);
			Weapon->VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SetEquippedWeapon(Weapon);
		}
	}
}

void ABetterPlayer::OutlineCheck(USphereComponent* CollisionVolume)
{
	TArray<AActor*> actors;
	CollisionVolume->GetOverlappingActors(actors, TSubclassOf<AObjectOutline>());
	float nearestDistance = FLT_MAX;
	HighlightActor = nullptr;
	for (auto actor : actors)
	{
		if (actor == this)
		{
			continue;
		}
		float dist = FVector::DistXY(GetActorLocation(), actor->GetActorLocation());
		if (dist < nearestDistance)
		{
			nearestDistance = dist;
			HighlightActor = Cast<AObjectOutline>(actor);
		}
	}
	if (HighlightActor)
	{
		HighlightActor->bOutlining = true;
	}
}

void ABetterPlayer::Pickup()
{
	HighlightActor->Pickup();
}