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
#include "ObjectOutline.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ShapeComponent.h"
#include "Pickup.h"
#include "Interactive.h"
#include "ElevatorSwitch.h"
#include "BadGuy.h"
#include "BetterPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Checkpoint.h"
#include "Torch.h"
#include "FogOfWarManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "MyGameInstance.h"

// Sets default values
ABetterPlayer::ABetterPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Face = CreateDefaultSubobject<USkeletalMeshComponent>("Face");
	Face->SetupAttachment(GetMesh());
	Hair = CreateDefaultSubobject<USkeletalMeshComponent>("Hair");
	Hair->SetupAttachment(GetMesh());
	Gloves = CreateDefaultSubobject<USkeletalMeshComponent>("Gloves");
	Gloves->SetupAttachment(GetMesh());
	Shoes = CreateDefaultSubobject<USkeletalMeshComponent>("Shoes");
	Shoes->SetupAttachment(GetMesh());
	HeadGears = CreateDefaultSubobject<USkeletalMeshComponent>("HeadGears");
	HeadGears->SetupAttachment(GetMesh());
	ShoulderPad = CreateDefaultSubobject<USkeletalMeshComponent>("ShoulderPad");
	ShoulderPad->SetupAttachment(GetMesh());
	Belt = CreateDefaultSubobject<USkeletalMeshComponent>("Belt");
	Belt->SetupAttachment(GetMesh());
	
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

	HighlightActor.Init(nullptr, 2);

	bAttacking = false;
	bCombo = false;
	MaxComboCount = 3;
	ComboCount = 0;

	bDefending = false;

	// Initialization
	DoorOpenRotate = 0.0f;
	bDoorNearby = false;
	bIsCameraRotating = false;
	InteractingDoor = nullptr;
	bHasCombatTarget = false;

	MaxHealth = 100.0f;
	Health = MaxHealth;
	HealthPercentage = 100.0f;

	MontageBlendOutTime = 0.25f;
	bWeapon = false;
	WeaponType = EWeaponType::EMS_NoWeapon;

	InterpSpeed = 15.0f;
	RollForce = 100.0f;

	bIsRolling = false;
	bComboTime = false;
}

float ABetterPlayer::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	bCombo = false;
	bAttacking = false;
	ComboCount = 0;
	bComboTime = false;
	bIsRolling = false;

	Health -= DamageAmount;
	AnimInstance->Montage_Play(CombatMontage);
	AnimInstance->Montage_JumpToSection("GetHit");
	if (Health < 0.0f)
	{
		AnimInstance->Montage_Play(CombatMontage);
		AnimInstance->Montage_JumpToSection("Die");
		//Die
	}

	UpdateHealth(Health);
	return DamageAmount;
}

// Called when the game starts or when spawned
void ABetterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	AnimInstance = GetMesh()->GetAnimInstance();
	BetterPlayerController = Cast<ABetterPlayerController>(GetController());

}

// Called every frame
void ABetterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OutlineCheck(EnemyCollisionVolume,1);
	OutlineCheck(ItemCollisionVolume,0);
	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		if (BetterPlayerController)
		{
			BetterPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}

	if (bInterpToEnemy && CombatTarget && bHasCombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);
	}

	if (bAttacking && bCombo && bComboTime)
	{
		SetInterpToEnemy(true);
		bAttacking = true;
		ComboCount++;
		bCombo = false;
		bComboTime = false;

		AnimInstance->Montage_Play(CombatMontage);
		FName SectionName;
		switch (ComboCount)
		{
		case 2:
			SectionName = FName("Combo02");
			break;
		case 3:
			SectionName = FName("Combo03");
			break;
		case 4:
			SectionName = FName("Combo04");
			break;
		case 5:
			SectionName = FName("Combo05");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, CombatMontage);
	}
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

	PlayerInputComponent->BindAction("Defend", EInputEvent::IE_Pressed, this, &ABetterPlayer::Defend);
	PlayerInputComponent->BindAction("Defend", EInputEvent::IE_Released, this, &ABetterPlayer::DefendEnd);

	PlayerInputComponent->BindAction("Skill", EInputEvent::IE_Pressed, this, &ABetterPlayer::Skill);
	PlayerInputComponent->BindAction("Roll", EInputEvent::IE_Pressed, this, &ABetterPlayer::Roll);

	PlayerInputComponent->BindAction("Save", EInputEvent::IE_Pressed, UMyGameInstance::GetInstance(), &UMyGameInstance::Save);
	PlayerInputComponent->BindAction("Load", EInputEvent::IE_Pressed, UMyGameInstance::GetInstance(), &UMyGameInstance::Load);
	

}

void ABetterPlayer::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f && !bAttacking && !bDefending)
	{
		const FVector Direction = FRotationMatrix(CameraBoom->GetRelativeRotation()).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction * 2.0f, Value);
	}
}

void ABetterPlayer::MoveSide(float Value)
{
	if (Controller != nullptr && Value != 0.0f && !bAttacking && !bDefending)
	{
		const FVector Direction = FRotationMatrix(CameraBoom->GetRelativeRotation()).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ABetterPlayer::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABetterPlayer::Attack()
{
	if (LeftEquippedWeapon || RightEquippedWeapon)
	{
		if (AnimInstance && CombatMontage && ComboCount < MaxComboCount && !bIsRolling)
		{
			if (AnimInstance->Montage_IsPlaying(CombatMontage) && !bCombo)
			{
				bCombo = true;
				return;
			}

			if (AnimInstance->Montage_GetIsStopped(CombatMontage) && ComboCount < MaxComboCount)
			{
				ComboCount++;
				bAttacking = true;
				SetInterpToEnemy(true);
				AnimInstance->Montage_Play(CombatMontage);
				AnimInstance->Montage_JumpToSection("Combo01", CombatMontage);
			}
		}

		
		/*if (AnimInstance && CombatMontage && ComboCount < MaxComboCount)
		{
			SetInterpToEnemy(true);
			bAttacking = true;
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
			case 4:
				SectionName = FName("Combo04");
				break;
			case 5:
				SectionName = FName("Combo05");
				break;
			default:
				break;
			}
			AnimInstance->Montage_JumpToSection(SectionName, CombatMontage);
		}*/
	}
}

void ABetterPlayer::Roll()
{
	if (AnimInstance && !bIsRolling)
	{
		AttackEnd();
		bIsRolling = true;
		AnimInstance->Montage_Stop(0.0f);
		AnimInstance->Montage_Play(CombatMontage);
		AnimInstance->Montage_JumpToSection("Roll", CombatMontage);

		float ForwardValue = GetInputAxisValue("MoveForward");
		float SideValue = GetInputAxisValue("MoveSide");
		if (ForwardValue != 0.0f || SideValue != 0.0f)
		{
			float YawDegree = UKismetMathLibrary::DegAtan2(SideValue, ForwardValue);
			SetActorRelativeRotation(FRotator(0.0f, YawDegree + CameraBoom->GetRelativeRotation().Yaw, 0.0f));
		}
	}
}

void ABetterPlayer::RollEnd()
{
	bIsRolling = false;
}

void ABetterPlayer::AttackEnd()
{
	bCombo = false;
	bAttacking = false;
	ComboCount = 0;
	bComboTime = false;
	SetInterpToEnemy(false);

	/*if (bCombo)
	{
		bCombo = false;
		ComboCount++;
	}
	else
	{
		AnimInstance->Montage_Stop(MontageBlendOutTime);
		bAttacking = false;
		ComboCount = 0;
		SetInterpToEnemy(false);
	}*/
}

void ABetterPlayer::SetEquippedWeapon(AWeapon * WeaponToSet, EEquippedWeapon Arm)
{
	if (Arm == EEquippedWeapon::EMS_LeftEquippedWeapon)
	{
		LeftEquippedWeapon = WeaponToSet;
		if (LeftEquippedWeapon)
		{
			CombatMontage = LeftEquippedWeapon->AnimMontage;
			MaxComboCount = LeftEquippedWeapon->MaxCombo;
			WeaponType = LeftEquippedWeapon->WeaponType;
			bWeapon = true;
		}
	}
	else
	{
		RightEquippedWeapon = WeaponToSet;
		if (RightEquippedWeapon)
		{
			CombatMontage = RightEquippedWeapon->AnimMontage;
			MaxComboCount = RightEquippedWeapon->MaxCombo;
			WeaponType = RightEquippedWeapon->WeaponType;
			bWeapon = true;
		}
	}
}

AWeapon * ABetterPlayer::GetEquippedWeapon(EEquippedWeapon EEquippedWeapon)
{
	if (EEquippedWeapon == EEquippedWeapon::EMS_LeftEquippedWeapon)
	{
		return LeftEquippedWeapon;
	}
	else
	{
		return RightEquippedWeapon;
	}
}

void ABetterPlayer::DebugEquip()
{
	/*if (EquippedWeapon == nullptr)
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
	}*/
}

void ABetterPlayer::Defend()
{
	if (AnimInstance && RightEquippedWeapon)
	{
		if (RightEquippedWeapon->WeaponType == EWeaponType::EMS_SwordShield)
		{
			AttackEnd();
			bDefending = true;
			AnimInstance->Montage_Play(CombatMontage);
			AnimInstance->Montage_JumpToSection("Defend");
		}
	}
}

void ABetterPlayer::DefendEnd()
{
	if (AnimInstance && bDefending)
	{
		bDefending = false;
		AnimInstance->Montage_Stop(0.25f);
	}
}

void ABetterPlayer::UpdateHealth(float AddValue)
{
	Health += AddValue;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	PreviousHealth = HealthPercentage;
	HealthPercentage = Health / MaxHealth;
}

void ABetterPlayer::Skill()
{
	if (AnimInstance && RightEquippedWeapon)
	{
		AttackEnd();
		SetInterpToEnemy(true);
		AnimInstance->Montage_Play(CombatMontage);
		AnimInstance->Montage_JumpToSection("Skill");
	}
}

void ABetterPlayer::OutlineCheck(USphereComponent* CollisionVolume, int objectTypeIdx)
{
	TArray<AActor*> actors;
	CollisionVolume->GetOverlappingActors(actors, TSubclassOf<AObjectOutline>());
	float nearestDistance = FLT_MAX;
	HighlightActor[objectTypeIdx] = nullptr;
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
			HighlightActor[objectTypeIdx] = Cast<AObjectOutline>(actor);
		}
	}
	if (HighlightActor[objectTypeIdx])
	{
		if (HighlightActor[objectTypeIdx]->bEnableOutline)
		{
			HighlightActor[objectTypeIdx]->bOutlining = true;
		}
	}
}

void ABetterPlayer::Pickup()
{
	if (HighlightActor[0])
	{
		UE_LOG(LogTemp, Warning, TEXT("HActor Acquired"));

		AItem* SelectedItem = Cast<AItem>(HighlightActor[0]->GetOwner());
		SelectedItem->PickUp(this);
	}

	//Click E to open door or activate switch
	if (bDoorNearby && InteractingDoor != nullptr)
	{
		AInteractive* Door = Cast<AInteractive>(InteractingDoor);
		Door->InteractDoorOpen(DoorOpenRotate);
	}
	else if (bSwitchNearby && InteractingSwitch != nullptr)
	{
		AElevatorSwitch* Switch = Cast<AElevatorSwitch>(InteractingSwitch);
		Switch->ActivateSwitch();
	}
}

void ABetterPlayer::InteractStart(float TargetRotation, bool Boolean, AActor* Door)
{
	//Register nearby door
	DoorOpenRotate = TargetRotation;
	bDoorNearby = Boolean;
	InteractingDoor = Door;
}

void ABetterPlayer::InteractStartSwitch(bool Boolean, AActor* Switch)
{
	//Register nearby elevator switch
	bSwitchNearby = Boolean;
	InteractingSwitch = Switch;
}

void ABetterPlayer::CameraRotatable(bool Boolean, AActor* Rotator)
{
	bIsCameraRotating = Boolean;
	InteractingRotator = Rotator;
}

AActor * ABetterPlayer::GetLastRotator()
{
	if (InteractingRotator)
	{
		return InteractingRotator;
	}
	return nullptr;
}

void ABetterPlayer::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

void ABetterPlayer::Die()
{
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.0f);
		AnimInstance->Montage_JumpToSection("Death");
	}
}

void ABetterPlayer::NextCombo()
{
	bComboTime = true;
}

void ABetterPlayer::SetEquippedShield(AShield* ShieldToSet)
{
	if (EquippedShield != nullptr)
	{
		EquippedShield = ShieldToSet;
	}
}

AShield* ABetterPlayer::GetEquippedShield()
{
	return EquippedShield;
}

FRotator ABetterPlayer::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.0f, LookAtRotation.Yaw, 0.0f);
	return LookAtRotationYaw;
}

