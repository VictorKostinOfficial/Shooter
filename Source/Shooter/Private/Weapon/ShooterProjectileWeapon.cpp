// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Weapon/ShooterProjectileBase.h"

void AShooterProjectileWeapon::Shoot_Implementation(const FVector& TraceEnd)
{
    Super::Shoot_Implementation(TraceEnd);

    // const USkeletalMeshSocket* MuzzleFlashSocket = WeaponMesh->GetSocketByName(FName("MuzzleFlash"));
    const FVector MuzzleLocation = WeaponMesh->GetSocketLocation(MuzzleSocketName);
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnParams.Instigator = Cast<APawn>(GetOwner());

    FRotator ProjRotation = (TraceEnd - MuzzleLocation).Rotation();

    FTransform SpawnTM = FTransform(ProjRotation, MuzzleLocation);
    GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

}