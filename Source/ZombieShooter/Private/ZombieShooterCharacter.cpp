// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ZombieShooter.h"
#include "ZombieShooterCharacter.h"
#include "ZombieShooterPlayerController.h"
#include "Projectile.h"
#include "UnrealNetwork.h"

AZombieShooterCharacter::AZombieShooterCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;

	PrimaryActorTick.bCanEverTick = true;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	bAttacking = false;
	bPressedProne = false;
	AttackCooldownRemaining = AttackCooldownBase;

	GetCharacterMovement()->AirControl = 1;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;

	MaxHealth = 100;
	CurrentHealth = MaxHealth;

	CurrentWeapon = 1;

	MaxCameraDistance = 1600.0f;
	MinCameraDistance = 400.0f;

	Rotation = FRotator::ZeroRotator;

	TargetedObject = NULL;
	Projectile = NULL;
}

void AZombieShooterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CalculateFacing();

	// Determine if the character should perform an attack
	AttackCooldownRemaining -= DeltaSeconds;
	if (bAttacking){
		PerformAttack();
	}
}

void AZombieShooterCharacter::CalculateFacing()
{
	// Rotate character towards the mouse pointer
	FVector PlaneNormal;
	PlaneNormal.X = 0.f;
	PlaneNormal.Y = 0.f;
	PlaneNormal.Z = 1.f;

	FVector PlanePoint = GetActorLocation();

	float PlaneFormulaD = -1 * (PlaneNormal.X * PlanePoint.X + PlaneNormal.Y * PlanePoint.Y + PlaneNormal.Z * PlanePoint.Z);

	// Calculate our t to calculate coordinates:
	float tLeftSide = PlaneNormal.X*WorldDirection.X + PlaneNormal.Y*WorldDirection.Y + PlaneNormal.Z*WorldDirection.Z;
	float tRightSide = -1 * (PlaneFormulaD + PlaneNormal.X*WorldLocation.X + PlaneNormal.Y*WorldLocation.Y + PlaneNormal.Z*WorldLocation.Z);
	float t = tRightSide / tLeftSide;

	FVector IntersectionPoint; // The result point where the mouse cursor is "projected". This is the point at which the pawn will be looking at.
	IntersectionPoint.X = t * WorldDirection.X + WorldLocation.X;
	IntersectionPoint.Y = t * WorldDirection.Y + WorldLocation.Y;
	IntersectionPoint.Z = t * WorldDirection.Z + WorldLocation.Z;

	FVector Direction = IntersectionPoint - PlanePoint;
	FRotator NewControlRotation = Direction.Rotation();
	NewControlRotation.Pitch = 0.f;
	NewControlRotation.Yaw = FRotator::ClampAxis(NewControlRotation.Yaw);
	NewControlRotation.Roll = 0.f;
	Rotation = NewControlRotation;
	ChangeFacing(Rotation);
}

void AZombieShooterCharacter::ChangeFacing(FRotator Rotation)
{
	SetActorRotation(Rotation);
	if (Role < ROLE_Authority) { ServerChangeFacing(Rotation); }
}

bool AZombieShooterCharacter::ServerChangeFacing_Validate(FRotator Rotation){
	return true;
}

void AZombieShooterCharacter::ServerChangeFacing_Implementation(FRotator Rotation){
	ChangeFacing(Rotation);
}

void AZombieShooterCharacter::PerformAttack()
{
	if (AttackCooldownRemaining >= 0.0f) return;

	if (Role < ROLE_Authority) 
	{ 
		ServerPerformAttack(); 
	}
	else
	{
		//Perform the actual attack here
		UE_LOG(LogTemp, Warning, TEXT("Performing attack!"));

		//In final product, will call the currently equipped weapon's fire function
		//For now, just fire a basic projectile
		if (ProjectileClass != NULL)
		{
			//MuzzleRotation.Pitch += 10.0f;
			UWorld* const World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				//Spawn projectile at muzzle
				FVector FireLocation = GetActorLocation() + FTransform(GetActorRotation()).TransformVector(FVector(25.0f, 0.0f, 0.0f));
				Projectile = World->SpawnActor<AProjectile>(ProjectileClass, FireLocation, GetActorRotation(), SpawnParams);
				
				if (Projectile)
				{
					//Find launch direction
					FVector const LaunchDir = GetActorRotation().Vector();
					//FVector const LaunchDir = CameraBoom->GetComponentRotation().Vector();
					Projectile->SetReplicates(true);
					Projectile->InitVelocity(LaunchDir);
				}
			}
		}
	}

	AttackCooldownRemaining = AttackCooldownBase;
	
}

void AZombieShooterCharacter::ServerPerformAttack_Implementation(){
	//Perform the actual attack here
	UE_LOG(LogTemp, Warning, TEXT("Server Performing attack!"));

	//In final product, will call the currently equipped weapon's fire function
	//For now, just fire a basic projectile
	if (ProjectileClass != NULL)
	{
		//MuzzleRotation.Pitch += 10.0f;
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			//Spawn projectile at muzzle
			FVector FireLocation = GetActorLocation() + FTransform(GetActorRotation()).TransformVector(FVector(25.0f, 0.0f, 0.0f));
			Projectile = World->SpawnActor<AProjectile>(ProjectileClass, FireLocation, GetActorRotation(), SpawnParams);

			if (Projectile)
			{
				//Find launch direction
				FVector const LaunchDir = GetActorRotation().Vector();
				//FVector const LaunchDir = CameraBoom->GetComponentRotation().Vector();
				Projectile->SetReplicates(true);
				Projectile->InitVelocity(LaunchDir);
			}
		}
	}
}

bool AZombieShooterCharacter::ServerPerformAttack_Validate(){
	return true;
}

float AZombieShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Applying Damage!"));
	CurrentHealth -= ActualDamage;

	if (CurrentHealth <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Killing the player!"));
	}
	return ActualDamage; 
}

void AZombieShooterCharacter::ZoomCamera(float Value)
{
	if (CameraBoom->TargetArmLength <= MaxCameraDistance && Value > 0.0f)
	{
		CameraBoom->TargetArmLength += Value;
	}
	else if (CameraBoom->TargetArmLength >= MinCameraDistance && Value < 0.0f)
	{
		CameraBoom->TargetArmLength += Value;
	}
}

void AZombieShooterCharacter::RotateCamera(float Value)
{
	const FRotator BoomRotation = CameraBoom->GetComponentRotation();
	CameraBoom->AddRelativeRotation(FRotator(0.0f, Value, 0.0f));
}

void AZombieShooterCharacter::MoveRight(float Value)
{
	const FRotator Rotation = TopDownCameraComponent->GetComponentRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AZombieShooterCharacter::MoveUp(float Value)
{
	const FRotator Rotation = TopDownCameraComponent->GetComponentRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AZombieShooterCharacter::StartSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AZombieShooterCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AZombieShooterCharacter::StartWalking()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AZombieShooterCharacter::StopWalking()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AZombieShooterCharacter::WeaponSwitch(uint16 Value)
{
	if (Value == CurrentWeapon) return;
	UE_LOG(LogTemp, Warning, TEXT("Hiding Weapon %d!"), CurrentWeapon);
	CurrentWeapon = Value;
	UE_LOG(LogTemp, Warning, TEXT("Equipped Weapon %d!"), CurrentWeapon);
}

void AZombieShooterCharacter::Interact()
{
	if (TargetedObject)
	{
		TargetedObject->Interact(this);
	}
}

void AZombieShooterCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AZombieShooterCharacter, Rotation);
	DOREPLIFETIME(AZombieShooterCharacter, Projectile);
}