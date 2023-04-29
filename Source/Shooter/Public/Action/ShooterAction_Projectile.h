// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Action/ShooterAction.h"
#include "ShooterAction_Projectile.generated.h"

class UAnimMontage;
class UParticleSystem;

/**
 * 
 */
UCLASS(Blueprintable)
class SHOOTER_API UShooterAction_Projectile : public UShooterAction
{
	GENERATED_BODY()
	
public:

	UShooterAction_Projectile();

	virtual void StartAction_Implementation(AActor* Instigator);

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName StartPointSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackAnim;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UParticleSystem> CastingEffect;

	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
};
