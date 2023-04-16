// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterWeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWeaponChanged, AActor*, InstigatorActor, UShooterWeaponComponent*, OwningComp, AActor*, NewWeapon);

class AShooterWeaponBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere, Category = "_Weapon")
	FName SockedNameToAttach;

	UFUNCTION(BlueprintCallable)
	void SetIsAiming(bool Value);

	UFUNCTION(BlueprintCallable)
	bool GetIsAiming();

	UFUNCTION(BlueprintCallable)
	bool ApplyWeapon(AActor* InstigatorActor, AActor* NewWeapon);

	UFUNCTION(BlueprintCallable)
	static UShooterWeaponComponent* GetWeaponComponent(AActor* FromActor);

	UFUNCTION(BlueprintCallable)
	AActor* GetEquippedWeapon();

	UFUNCTION(BlueprintCallable)
	bool IsWeaponEquipped();


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "_Weapon")
	TObjectPtr<AActor> EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "_Weapon")
	bool bIsAiming;

	UFUNCTION(Server, Reliable)
	void Server_SetIsAiming(bool NewValue);

	UPROPERTY(BlueprintAssignable, Category = "_Weapon")
	FOnWeaponChanged OnWeaponChanged;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_WeaponChanged(AActor* InstigatorActor, AActor* NewWeapon);

public:	

	UShooterWeaponComponent();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
