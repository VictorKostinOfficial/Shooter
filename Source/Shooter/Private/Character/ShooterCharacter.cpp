#include "Character/ShooterCharacter.h"

// Components
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"

// Custom Components
#include "Character/ShooterInteractionComponent.h"
#include "Weapon/ShooterWeaponComponent.h"

// Input systems
#include "EnhancedInputComponent.h" 
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"


#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x);}


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

	InteractionComponent = CreateDefaultSubobject<UShooterInteractionComponent>("Interaction Component");
	WeaponComponent = CreateDefaultSubobject<UShooterWeaponComponent>("Weapon Component");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	bUseControllerRotationYaw = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

}


void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerControl = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControl->GetLocalPlayer()); // That works
	InputSystem->ClearAllMappings();
	InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(InputMove.LoadSynchronous(), ETriggerEvent::Triggered, this, &AShooterCharacter::Move);
	Input->BindAction(InputLook.LoadSynchronous(), ETriggerEvent::Triggered, this, &AShooterCharacter::Look);
	Input->BindAction(InputJump.LoadSynchronous(), ETriggerEvent::Triggered, this, &AShooterCharacter::Jumped);
	Input->BindAction(InputCrouch.LoadSynchronous(), ETriggerEvent::Triggered, this, &AShooterCharacter::Crouched);
	Input->BindAction(InputAiming.LoadSynchronous(), ETriggerEvent::Triggered, this, &AShooterCharacter::Aiming);

	Input->BindAction(InputInteraction.LoadSynchronous(), ETriggerEvent::Triggered, this, &AShooterCharacter::PrimaryInteract);
	Input->BindAction(InputShoot.LoadSynchronous(), ETriggerEvent::Triggered, this, &AShooterCharacter::PrimaryShoot);

}


void AShooterCharacter::Look(const FInputActionValue &Value)
{
	if (Controller)
	{
		const FVector2D InputVector = Value.Get<FVector2D>();

		// Left/Right look
		if (InputVector.X != 0.f)
			AddControllerYawInput(InputVector.X);

		// Up/Down look
		if (InputVector.Y != 0.f)
			AddControllerPitchInput(InputVector.Y);
	}
}


void AShooterCharacter::Jumped()
{
	Jump();
}


void AShooterCharacter::Crouched()
{
	bIsCrouched ? UnCrouch() : Crouch();
}


void AShooterCharacter::PrimaryInteract()
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

void AShooterCharacter::Aiming()
{
	if (WeaponComponent != nullptr)
	{
		WeaponComponent->GetIsAiming() ? WeaponComponent->SetIsAiming(false) : WeaponComponent->SetIsAiming(true);
	}
}


void AShooterCharacter::PrimaryShoot()
{
	if (WeaponComponent->GetEquippedWeapon() == nullptr)
	{
		return;
	}


	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, WeaponComponent->GetEquippedWeapon()->GetName() + " : Shoooooot!");

}


bool AShooterCharacter::IsWeaponEquipped()
{
    return (WeaponComponent != nullptr && WeaponComponent->IsWeaponEquipped());
}


bool AShooterCharacter::IsAiming()
{
	return (WeaponComponent && WeaponComponent->GetIsAiming());
}


void AShooterCharacter::Move(const FInputActionValue& Value)
{
	if (Controller)
	{
		const FVector InputVector = Value.Get<FVector>(); 
		const FVector MoveVector = InputVector.RotateAngleAxis(GetControlRotation().Yaw, FVector::UpVector);

		AddMovementInput(MoveVector);
		
	}
}