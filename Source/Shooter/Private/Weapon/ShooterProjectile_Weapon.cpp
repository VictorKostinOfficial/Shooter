// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/ShooterProjectile_Weapon.h"
#include "Components/BoxComponent.h"
#include "ShooterFunctionLibrary.h"
#include "Character/ShooterAttributeComponent.h"

AShooterProjectile_Weapon::AShooterProjectile_Weapon()
{
    InitialLifeSpan = 5.f;
    DamageAmount = 30.f;
}

void AShooterProjectile_Weapon::PostInitializeComponents()
{
    Super::PostInitializeComponents();

}

void AShooterProjectile_Weapon::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AShooterProjectile_Weapon::OnActorHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
        UShooterFunctionLibrary::ApplyDamage(GetInstigator(), OtherActor, DamageAmount);
	}
    Explode();
}