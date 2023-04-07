#include "Character/ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "EnhancedInputComponent.h" // Plugin for new input system

AShooterCharacter::AShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);	
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void AShooterCharacter::MoveForward(float Value)
{
	FRotator RotationControl = GetControlRotation();
	RotationControl.Pitch = 0.0f;
	RotationControl.Roll = 0.0f;

	AddMovementInput(RotationControl.Vector(), Value);
}

void AShooterCharacter::MoveRight(float Value)
{
	FRotator RotationControl = GetControlRotation();
	RotationControl.Pitch = 0.0f;
	RotationControl.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(RotationControl).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);
}

