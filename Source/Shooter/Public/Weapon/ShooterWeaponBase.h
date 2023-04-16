// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterInteractionInterface.h"

#include "ShooterWeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponState2 : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX"),
};

UCLASS()
class SHOOTER_API AShooterWeaponBase : public AActor, public IShooterInteractionInterface
{
	GENERATED_BODY()
	
public:	

	void Interact_Implementation(APawn* InstigatorPawn);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UFUNCTION()
	void OnRep_WeaponState();

private:

	UPROPERTY(VisibleAnywhere, Category = "_Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "_Weapon")
	EWeaponState2 WeaponState;

public:	

	AShooterWeaponBase();

};
