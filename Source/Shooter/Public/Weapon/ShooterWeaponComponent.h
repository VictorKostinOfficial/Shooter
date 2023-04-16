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

	UPROPERTY(BlueprintAssignable, Category = "_Weapon")
	FOnWeaponChanged OnWeaponChanged;

	UPROPERTY(EditAnywhere, Category = "_Weapon")
	FName SockedNameToAttach;

	UFUNCTION(BlueprintCallable, Category = "_Weapon")
	bool ApplyWeapon(AActor* InstigatorActor, AActor* NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "_Weapon")
	static UShooterWeaponComponent* GetWeaponComponent(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "_Weapon")
	AActor* GetEquippedWeapon();

protected:

	TObjectPtr<ACharacter> Character;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "_Weapon")
	TObjectPtr<AActor> EquippedWeapon;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_WeaponChanged(AActor* InstigatorActor, AActor* NewWeapon);

public:	

	UShooterWeaponComponent();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
