// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurningInPlace.h"
#include "ShooterWeaponComponent.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWeaponChanged, AActor*, InstigatorActor, UShooterWeaponComponent*, OwningComp, AActor*, NewWeapon);

class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterWeaponComponent : public UActorComponent
{
	
	GENERATED_BODY()

public:	
	friend class AShooterCharacter;

	UPROPERTY(EditAnywhere, Category = "_Weapon")
	FName SockedNameToAttach;

	UPROPERTY(EditAnywhere, Category = "_Weapon")
	TObjectPtr<UAnimMontage> FireWeaponMontage;



	// Manage weapon properties
	UFUNCTION(BlueprintCallable)
	FName GetWeaponSocketName();

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetWeaponSkeletalMeshComponent();

	UFUNCTION(BlueprintCallable)
	EWeaponState GetWeaponState();

	UFUNCTION(BlueprintCallable)
	void Drop();



	// Component functions

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

	ETurningInPlace GetTurningInPlace();

	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }

	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }


protected:

	// PROPERTIES
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACharacter> Character;

	UPROPERTY(VisibleAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_EquippedWeapon, Category = "_Weapon")
	TObjectPtr<AActor> EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "_Weapon")
	bool bIsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "_Weapon")
	bool bIsShooting;

	UPROPERTY(VisibleAnywhere)
	float AO_Yaw;

	UPROPERTY(VisibleAnywhere)
	float AO_Pitch;

	UPROPERTY(VisibleAnywhere)
	float InterpAO_Yaw;

	UPROPERTY(VisibleAnywhere)
	FRotator AimRotation;

	UPROPERTY(VisibleAnywhere)
	ETurningInPlace TurningInPlace;



	// FUNCTIONS
	UPROPERTY(EditAnywhere, Category = "_Weapon")
	float AimWalkSpeed;

	UFUNCTION(BlueprintCallable)
	void SetIsAiming(bool Value);

	void AimOffset(float DeltaTime);

	void SetIsShooting(bool Value);

	void PlayFireMontage(bool bIsShooting);

	UFUNCTION(Server, Reliable)
	void Server_Shoot(bool Value, const FVector_NetQuantize TraceHit);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Shoot(bool Value, const FVector_NetQuantize TraceHit);

	void TraceCenter(FHitResult& TraceHitResult);

	void TurnInPlace(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void Server_SetIsAiming(bool NewValue);

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
