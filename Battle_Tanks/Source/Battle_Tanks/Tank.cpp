// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "Turret.h"
#include "Engine/World.h"
#include "Projectile.h"
#include "TankBarrel.h"
#include "TankMovementComponent.h"

// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Tank_Aiming_Component"));
	
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

}



// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATank::AimAt(FVector HitLocation)
{
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);

}

void ATank::SetBarrelReference(UTankBarrel * Barreltoset)
{
	Barrel = Barreltoset;
	TankAimingComponent->SetBarrelReference(Barreltoset);
}

void ATank::SetTurretReference(UTurret * Turrettoset)
{

	TankAimingComponent->SetTurretReference(Turrettoset);
}

void ATank::Fire()
{
	if (!Barrel) { return; }

	 isReloaded = (FPlatformTime::Seconds()- LastFireTime) > ReloadTimeInSeconds;


	if (isReloaded)
	{
		auto projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation(FName("FireSpawn")), Barrel->GetSocketRotation(FName("FireSpawn")));
		projectile->Launchprojectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
	

	
	
}
