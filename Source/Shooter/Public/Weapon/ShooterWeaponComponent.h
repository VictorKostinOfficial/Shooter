// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurningInPlace.h"
#include "ShooterWeaponComponent.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWeaponChanged, AActor*, InstigatorActor, UShooterWeaponComponent*, OwningComp, AActor*, NewWeapon);

class ACharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterWeaponComponent : public UActorComponent
{
	
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere, Category = "_Weapon")
	FName SockedNameToAttach;

	UPROPERTY(EditAnywhere, Category = "_Weapon")
	float AimWalkSpeed;



	// Manage weapon properties
	UFUNCTION(BlueprintCallable)
	FName GetWeaponSocketName();

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetWeaponSkeletalMeshComponent();

	UFUNCTION(BlueprintCallable)
	EWeaponState GetWeaponState();



	// Component functions
	UFUNCTION(BlueprintCallable)
	void SetIsAiming(bool Value);

	UFUNCTION(BlueprintCallable)
	bool GetIsAiming();

	UFUNCTION(BlueprintCallable)
	bool ApplyWeapon(AActor* NewWeapon);

	UFUNCTION(BlueprintCallable)
	static UShooterWeaponComponent* GetWeaponComponent(AActor* FromActor);

	UFUNCTION(BlueprintCallable)
	AActor* GetEquippedWeapon();

	UFUNCTION(BlueprintCallable)
	bool IsWeaponEquipped();

	void AimOffset(float DeltaTime);

	ETurningInPlace GetTurningInPlace();

	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }

	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }


protected:

	TObjectPtr<ACharacter> Character;

	float BaseWalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_EquippedWeapon, Category = "_Weapon")
	TObjectPtr<AActor> EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "_Weapon")
	bool bIsAiming;

	UPROPERTY(Replicated)
	float AO_Yaw;

	float AO_Pitch;

	FRotator AimRotation;

	ETurningInPlace TurningInPlace;


	void TurnInPlace(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void Server_SetIsAiming(bool NewValue);

	UFUNCTION(Server, Unreliable)
	void Server_SetAO_Yaw(float AO_Yaw_New);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	// For Event
	// UPROPERTY(BlueprintAssignable, Category = "_Weapon")
	// FOnWeaponChanged OnWeaponChanged;

	// UFUNCTION(NetMulticast, Reliable)
	// void Multicast_WeaponChanged(AActor* InstigatorActor, AActor* NewWeapon);

public:	

	UShooterWeaponComponent();

	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
