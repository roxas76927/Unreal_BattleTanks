// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "TankProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	TankProjectileMovementComponent = CreateDefaultSubobject<UTankProjectileMovementComponent>(FName("Projectile_Movement_Component_FromCPP"));
	TankProjectileMovementComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Launchprojectile(float speed)
{
	TankProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * speed);
	TankProjectileMovementComponent->Activate();
}


