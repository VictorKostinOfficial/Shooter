// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterInteractionInterface.h"

#include "WeaponBase.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class EWeaponState3 : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX"),
};

UCLASS()
class SHOOTER_API AWeaponBase : public AActor, public IShooterInteractionInterface
{
	GENERATED_BODY()
	
public:
	
	void Interact_Implementation(APawn* InstigatorPawn);

	UFUNCTION()
	void OnRep_WeaponState();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "_Weapon")
	int AmmoMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "_Weapon")
	int AmmoCurrent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "_Weapon")
	float ShootDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "_Weapon")
	float ReloadDelay;

private:

	UPROPERTY(VisibleAnywhere, Category = "_Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "_Weapon")
	EWeaponState3 WeaponState;

public:	
	AWeaponBase();

	virtual void Tick(float DeltaTime) override;

};
