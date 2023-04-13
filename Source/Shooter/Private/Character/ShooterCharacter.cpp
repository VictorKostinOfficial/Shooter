#include "Character/ShooterCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"

// Plugins for new input system
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
	bUseControllerRotationYaw = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);
}

void AShooterCharacter::Move(const FInputActionValue& Value)
{
	if (Controller)
	{
		const FVector InputVector = Value.Get<FVector>(); 
		const FVector test = InputVector.RotateAngleAxis(GetControlRotation().Yaw, FVector::UpVector);

		AddMovementInput(test);
		
		// Forward/Backward direction
    	//if (InputVector.Y != 0.f)
    	//{
        	//const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
        	//AddMovementInput(Direction, InputVector.Y);
    	//}
 
    	//// Right/Left direction
    	//if (InputVector.X != 0.f)
    	//{
        	//const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
        	//AddMovementInput(Direction, InputVector.X);
    	//}
	}
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
	Input->BindAction(InputJump.LoadSynchronous(), ETriggerEvent::Triggered, this, &ACharacter::Jump);

}