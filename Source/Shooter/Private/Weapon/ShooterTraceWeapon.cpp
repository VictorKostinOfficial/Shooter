// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterTraceWeapon.h"

void AShooterTraceWeapon::Shoot_Implementation(const FVector &HitTarget)
{
    const FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
}
