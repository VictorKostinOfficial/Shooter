// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponStates.h"

#include "ShooterWeaponInterface.generated.h"


UINTERFACE(MinimalAPI)
class UShooterWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class SHOOTER_API IShooterWeaponInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FName GetSocketName();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USkeletalMeshComponent* GetSkeletalMeshComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EWeaponState GetWeaponState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Shoot(const FVector& HitTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Drop();
};
