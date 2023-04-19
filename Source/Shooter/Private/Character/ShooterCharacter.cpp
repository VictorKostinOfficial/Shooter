#include "Character/ShooterCharacter.h"

// Components
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	bUseControllerRotationYaw = false;

	InteractionComponent = CreateDefaultSubobject<UShooterInteractionComponent>("Interaction Component");
	WeaponComponent = CreateDefaultSubobject<UShooterWeaponComponent>("Weapon Component");

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}


void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WeaponComponent != nullptr)
	{
		WeaponComponent->AimOffset(DeltaTime);
	}
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


float AShooterCharacter::GetAO_Yaw()
{
	if (WeaponComponent == nullptr) return 0;
    return WeaponComponent->GetAO_Yaw();
}


float AShooterCharacter::GetAO_Pitch()
{
	if (WeaponComponent == nullptr) return 0;
    return WeaponComponent->GetAO_Pitch();
}


ETurningInPlace AShooterCharacter::GetTurningInPlace()
{
	if (WeaponComponent == nullptr) return ETurningInPlace::ETIP_None;
    return WeaponComponent->GetTurningInPlace();
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
		D(TEXT("BLYYYY"));
		return;
	}


	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, WeaponComponent->GetEquippedWeapon()->GetName() + " : Shoooooot!");

}


bool AShooterCharacter::IsWeaponEquipped()
{
    return (WeaponComponent != nullptr && WeaponComponent->IsWeaponEquipped());
}


AActor* AShooterCharacter::GetEquippedWeapon()
{
	if (IsWeaponEquipped()) 
	{
		WeaponComponent->GetEquippedWeapon();
	}
    return nullptr;
}


FName AShooterCharacter::GetWeaponSocketName()
{
	if (WeaponComponent == nullptr) return "";
    return WeaponComponent->GetWeaponSocketName();
}


USkeletalMeshComponent *AShooterCharacter::GetWeaponSkeletalMeshComponent()
{
	if (WeaponComponent == nullptr) return nullptr;
    return WeaponComponent->GetWeaponSkeletalMeshComponent();
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
