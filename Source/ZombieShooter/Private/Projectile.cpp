// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieShooter.h"
#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ZombieCharacter.h"
#include "Engine.h"
#include "UnrealNetwork.h"


AProjectile::AProjectile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Use a sphere as a simple collision representation
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 300.f;
	ProjectileMovement->MaxSpeed = 300.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;

	//CollisionComp->SetCollisionProfileName(FName(TEXT("BlockAllDynamic")));
	//CollisionComp->OnComponentHit = AProjectile::OnHit()
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	InitialLifeSpan = 3.0f;
}

void AProjectile::InitVelocity(const FVector& ShootDirection)
{
	if (ProjectileMovement)
	{
		// set the projectile's velocity to the desired direction
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}

void AProjectile::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);

		//AZombieCharacter* Zombie = Cast<AZombieCharacter>()

		//if (Cast())
		UE_LOG(LogTemp, Warning, TEXT("Hit!"));

		AZombieCharacter* Zombie = Cast<AZombieCharacter>(OtherComp->GetOwner());
		if (Zombie)
		{
			Zombie->TakeDamage(25, FDamageEvent(), NULL, this);
		}

		UWorld* const World = GetWorld();
		if (World)
		{
			World->DestroyActor(this);
		}
	}
}
