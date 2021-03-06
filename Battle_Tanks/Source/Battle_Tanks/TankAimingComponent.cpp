// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "Engine/World.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()

{

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features

	// off to improve performance if you don't need them.

	//bWantsBeginPlay = true;

	PrimaryComponentTick.bCanEverTick = true;

}



void UTankAimingComponent::BeginPlay()

{

	// So that first first is after initial reload

	LastFireTime = FPlatformTime::Seconds();

}



void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTurret* TurretToSet)

{

	Barrel = BarrelToSet;

	Turret = TurretToSet;

}



void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)

{
	if (rounds <= 0)
	{
		FiringState = FiringState::Empty;

	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)

	{

		FiringState = FiringState::Reloading;

	}

	else if (IsBarrelMoving())

	{

		FiringState = FiringState::Aiming;

	}

	else

	{

		FiringState = FiringState::Locked;

	}

}



bool UTankAimingComponent::IsBarrelMoving()

{

	if (!ensure(Barrel)) { return false; }

	auto BarrelForward = Barrel->GetForwardVector();

	return !BarrelForward.Equals(AimDirection, 0.01); // vectors are equal

}



void UTankAimingComponent::AimAt(FVector HitLocation)

{

	if (!ensure(Barrel)) { return; }



	FVector OutLaunchVelocity;

	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity

	(

		this,

		OutLaunchVelocity,

		StartLocation,

		HitLocation,

		LaunchSpeed,

		false,

		0,

		0,

		ESuggestProjVelocityTraceOption::DoNotTrace // Paramater must be present to prevent bug

	);



	if (bHaveAimSolution)

	{

		AimDirection = OutLaunchVelocity.GetSafeNormal();

		MoveBarrelTowards(AimDirection);

	}

	// If no solution found do nothing

}

int UTankAimingComponent::getrounds()
{
	return rounds;
}



void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)

{

	if (!ensure(Barrel) || !ensure(Turret)) { return; }



	// Work-out difference between current barrel roation, and AimDirection

	auto BarrelRotator = Barrel->GetForwardVector().Rotation();

	auto AimAsRotator = AimDirection.Rotation();

	auto DeltaRotator = AimAsRotator - BarrelRotator;



	// Always yaw the shortest way

	Barrel->Elevate(DeltaRotator.Pitch);

	if (FMath::Abs(DeltaRotator.Yaw) < 180)

	{

		Turret->RotateTurret(DeltaRotator.Yaw);

	}

	else

	{

		Turret->RotateTurret(-DeltaRotator.Yaw);

	}

}



void UTankAimingComponent::Fire()

{

	if (FiringState == FiringState::Locked || FiringState == FiringState::Aiming)

	{

		// Spawn a projectile at the socket location on the barrel

		if (!ensure(Barrel)) { return; }

		if (!ensure(ProjectileBlueprint)) { return; }

		auto Projectile = GetWorld()->SpawnActor<AProjectile>(

			ProjectileBlueprint,

			Barrel->GetSocketLocation(FName("Projectile")),

			Barrel->GetSocketRotation(FName("Projectile"))

			);

		rounds = rounds - 1;
		if (rounds <= 0)
		FiringState = FiringState::Empty;
		UE_LOG(LogTemp, Warning, TEXT("Ammo Left: %i"), rounds);


		Projectile->Launchprojectile(LaunchSpeed);
				LastFireTime = FPlatformTime::Seconds();

	}

}


FiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}
