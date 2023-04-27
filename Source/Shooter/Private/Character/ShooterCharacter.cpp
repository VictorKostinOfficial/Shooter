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
#include "Character/ShooterAttributeComponent.h"
#include "Action/ShooterActionComponent.h"
#include "Character/ShooterAnimInstance.h"

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
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 850.f);
	bUseControllerRotationYaw = false;

	InteractionComponent = CreateDefaultSubobject<UShooterInteractionComponent>("Interaction Component");
	WeaponComponent = CreateDefaultSubobject<UShooterWeaponComponent>("Weapon Component");
	AttributeComponent = CreateDefaultSubobject<UShooterAttributeComponent>("Attribute Component");
	ActionComponent = CreateDefaultSubobject<UShooterActionComponent>("Action Component");

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
}


void AShooterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (WeaponComponent)
	{
		WeaponComponent->Character = this;
	}
	if (AttributeComponent)
	{
		// AttributeComponent->OnHealthChanged.AddDynamic(this, &AShooterCharacter::OnHealthChanged);
		AttributeComponent->OnPlayerDead.AddDynamic(this, &AShooterCharacter::OnPlayerDead);
	}
}


void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WeaponComponent != nullptr)
	{
		FHitResult Hit;
		WeaponComponent->AimOffset(DeltaTime);
		// WeaponComponent->TraceCenter(Hit);
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
	Input->BindAction(InputJump.LoadSynchronous(), ETriggerEvent::Started, this, &AShooterCharacter::Jump);
	Input->BindAction(InputInteraction.LoadSynchronous(), ETriggerEvent::Started, this, &AShooterCharacter::PrimaryInteract);

	Input->BindAction(InputCrouch.LoadSynchronous(), ETriggerEvent::Started, this, &AShooterCharacter::CrouchButtonIsPressed);		// Pressed
	Input->BindAction(InputCrouch.LoadSynchronous(), ETriggerEvent::Completed, this, &AShooterCharacter::UncrouchButtonIsReleased);	// Released

	Input->BindAction(InputAiming.LoadSynchronous(), ETriggerEvent::Started, this, &AShooterCharacter::AimingButtonIsPressed);
	Input->BindAction(InputAiming.LoadSynchronous(), ETriggerEvent::Completed, this, &AShooterCharacter::AimingButtonIsReleased);

	Input->BindAction(InputShoot.LoadSynchronous(), ETriggerEvent::Started, this, &AShooterCharacter::ShootingButtonIsPressed);
	Input->BindAction(InputShoot.LoadSynchronous(), ETriggerEvent::Completed, this, &AShooterCharacter::ShootingButtonIsReleased);

	Input->BindAction(InputQAbility.LoadSynchronous(), ETriggerEvent::Started, this, &AShooterCharacter::QButtonIsPressed);
	Input->BindAction(InputEAbility.LoadSynchronous(), ETriggerEvent::Started, this, &AShooterCharacter::EButtonIsPressed);
	Input->BindAction(InputZAbility.LoadSynchronous(), ETriggerEvent::Started, this, &AShooterCharacter::ZButtonIsPressed);
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


void AShooterCharacter::Jump()
{
	Super::Jump();
}


void AShooterCharacter::PrimaryInteract()
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}


void AShooterCharacter::CrouchButtonIsPressed()
{
	D(TEXT("Crouch Pressed"));
	if (GetMovementComponent()->IsFalling()) return;
	Super::Crouch();
}
void AShooterCharacter::UncrouchButtonIsReleased()
{
	D(TEXT("Crouch Released"));
	Super::UnCrouch();
}


void AShooterCharacter::AimingButtonIsPressed()
{
	if (WeaponComponent && WeaponComponent->IsWeaponEquipped())
	{
		D(TEXT("Aiming Pressed"));
		WeaponComponent->SetIsAiming(true);
	}
}
void AShooterCharacter::AimingButtonIsReleased()
{
	if (WeaponComponent && WeaponComponent->IsWeaponEquipped())
	{
		D(TEXT("Aiming Released"));
		WeaponComponent->SetIsAiming(false);
	}
}


void AShooterCharacter::ShootingButtonIsPressed()
{
	if (WeaponComponent && WeaponComponent->IsWeaponEquipped())
	{
		D(TEXT("Shoot Pressed"));
		WeaponComponent->SetIsShooting(true);
	}
}
void AShooterCharacter::ShootingButtonIsReleased()
{
	if (WeaponComponent && WeaponComponent->IsWeaponEquipped())
	{
		D(TEXT("Shoot Released"));
		WeaponComponent->SetIsShooting(false);
	}
}

void AShooterCharacter::QButtonIsPressed()
{
		D(TEXT("Q Pressed"));
}

void AShooterCharacter::EButtonIsPressed()
{
		D(TEXT("E Pressed"));
}

void AShooterCharacter::ZButtonIsPressed()
{
		D(TEXT("Z Pressed"));
}

void AShooterCharacter::PlayFireMontage(bool bIsAiming)
{
	if (WeaponComponent && WeaponComponent->IsWeaponEquipped())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if(AnimInstance && WeaponComponent->FireWeaponMontage)		
		{
			if(!bIsAiming)
			{
				AnimInstance->Montage_Stop(0.2f, WeaponComponent->FireWeaponMontage);
				return;
			}

			AnimInstance->Montage_Play(WeaponComponent->FireWeaponMontage);
			FName SectionName;
			SectionName = bIsAiming ? FName("RifleAim") : FName("RifleHip");
			AnimInstance->Montage_JumpToSection(SectionName);
		}
	}
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

//void AShooterCharacter::OnHealthChanged(AActor *InstigatorActor, UShooterAttributeComponent *OwningComp, float NewHealth, float Delta)
//{
//}

void AShooterCharacter::OnPlayerDead(AActor * InstigatorActor, UShooterAttributeComponent * OwningComp)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	APlayerController* PC = Cast<APlayerController>(GetController());
	DisableInput(PC);

	if (IsWeaponEquipped())
	{
		WeaponComponent->Drop();
	}


	SetLifeSpan(5.f);
}