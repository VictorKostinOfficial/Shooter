// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterInteractionInterface.h"
#include "ShooterWeaponInterface.h"

#include "ShooterWeaponBase.generated.h"

class UAnimationAsset;

UCLASS()
class SHOOTER_API AShooterWeaponBase : public AActor, public IShooterInteractionInterface, public IShooterWeaponInterface
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleAnywhere, Category = "_Weapon")
	FName LeftHandSocketName;

	UPROPERTY(VisibleAnywhere, Category = "_Weapon")
	FName MuzzleSocketName;


	// WeaponInterface
	FName GetSocketName_Implementation();

	USkeletalMeshComponent* GetSkeletalMeshComponent_Implementation();

	EWeaponState GetWeaponState_Implementation();

	void Shoot_Implementation(const FVector& HitTarget);

	void Drop_Implementation();



	// InteractionInterface
	void Interact_Implementation(APawn* InstigatorPawn);

protected:

	UPROPERTY(VisibleAnywhere, Category = "_Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "_Weapon")
	TObjectPtr<UAnimationAsset> FireAnimation;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "_Weapon")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();

public:	

	AShooterWeaponBase();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
