// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterWeaponBase.h"
#include "ShooterProjectileWeapon.generated.h"

class AShooterProjectileBase;

UCLASS()
class SHOOTER_API AShooterProjectileWeapon : public AShooterWeaponBase
{
	GENERATED_BODY()

public:

	void Shoot_Implementation(const FVector& HitTarget);
	
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AShooterProjectileBase> ProjectileClass;
};
