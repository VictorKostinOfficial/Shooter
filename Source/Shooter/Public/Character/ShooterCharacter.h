// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ShooterCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AShooterCharacter();

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	//void MoveForward(float Value);

	//void MoveRight(float Value);

private:

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(EditANywhere, Category = "Enhanced Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditANywhere, Category = "Enhanced Input")
	TSoftObjectPtr<UInputAction> InputMove;
	
	UPROPERTY(EditANywhere, Category = "Enhanced Input")
	TSoftObjectPtr<UInputAction> InputLook;

	UPROPERTY(EditANywhere, Category = "Enhanced Input")
	TSoftObjectPtr<UInputAction> InputJump;
};
