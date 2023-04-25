// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShooterProjectileBase.h"
#include "ShooterProjectile_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AShooterProjectile_Weapon : public AShooterProjectileBase
{
	GENERATED_BODY()
	
public:

	AShooterProjectile_Weapon();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void PostInitializeComponents() override;
};
