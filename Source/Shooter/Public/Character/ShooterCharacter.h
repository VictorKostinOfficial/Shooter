// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UWidgetComponent;
class UShooterInteractionComponent;
class UShooterWeaponComponent;
//class AWeaponBase;

struct FInputActionValue;

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AShooterCharacter();

	virtual void Tick(float DeltaTime) override;

	bool IsWeaponEquipped();

	bool IsAiming();

	void AimOffset(float DeltaTime);

	float GetAO_Yaw();

	float GetAO_Pitch();

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Jumped();

	void Crouched();

	void PrimaryInteract();

	void Aiming();

	void PrimaryShoot();

	float Base;

	float Camera;

	float AO_Yaw;

	float AO_Pitch;

	FRotator AimRotation;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* OverheadWidget;

	// Character components
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShooterInteractionComponent> InteractionComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShooterWeaponComponent> WeaponComponent;



	// User Input Actions
	UPROPERTY(EditANywhere, Category = "Enhanced Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditANywhere, Category = "Enhanced Input")
	TSoftObjectPtr<UInputAction> InputMove;
	
	UPROPERTY(EditANywhere, Category = "Enhanced Input")
	TSoftObjectPtr<UInputAction> InputLook;

	UPROPERTY(EditANywhere, Category = "Enhanced Input")
	TSoftObjectPtr<UInputAction> InputJump;

	UPROPERTY(EditANywhere, Category = "Enhanced Input")
	TSoftObjectPtr<UInputAction> InputInteraction;

	UPROPERTY(EditANywhere, Category = "Enhanced Input")
	TSoftObjectPtr<UInputAction> InputShoot;

	UPROPERTY(EditANywhere, Category = "Enhanced Input")
	TSoftObjectPtr<UInputAction> InputCrouch;

	UPROPERTY(EditANywhere, Category = "Enhanced Input")
	TSoftObjectPtr<UInputAction> InputAiming;
};
