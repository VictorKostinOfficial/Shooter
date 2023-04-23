// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterWeaponBase.h"
#include "ShooterProjectileWeapon.generated.h"

class AShooterProjectile;

UCLASS()
class SHOOTER_API AShooterProjectileWeapon : public AShooterWeaponBase
{
	GENERATED_BODY()

public:

	void Shoot_Implementation(const FVector& HitTarget);
	
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AShooterProjectile> ProjectileClass;
};
