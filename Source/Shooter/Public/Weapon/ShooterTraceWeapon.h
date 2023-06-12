// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterWeaponBase.h"
#include "ShooterTraceWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AShooterTraceWeapon : public AShooterWeaponBase
{
	GENERATED_BODY()

public:

	void Shoot_Implementation(const FVector& HitTarget);
	
};
