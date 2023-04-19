#include "Character/ShooterAnimInstance.h"

#include "Character/ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x);}
void UShooterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner() /*This return a Pawn class, needed to cast*/);
}

void UShooterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (ShooterCharacter == nullptr)
    {
        ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner() /*This return a Pawn class, needed to cast*/);
    }

    if (ShooterCharacter == nullptr) return;

    FVector Velocity = ShooterCharacter->GetVelocity();
    Velocity.Z = 0.f;
    SpeedAnimation = Velocity.Size();

    bIsIntAir = ShooterCharacter->GetCharacterMovement()->IsFalling();
    bIsAccelerating = ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
    bIsWeaponEquipped = ShooterCharacter->IsWeaponEquipped();
    bIsCrouched = ShooterCharacter->bIsCrouched;
    bIsAiming = ShooterCharacter->IsAiming();

    FRotator AimRotation = ShooterCharacter->GetBaseAimRotation();
    FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());

    FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
    DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
    YawOffset = DeltaRotation.Yaw;

    CharacterRotationLastFrame = CharacterRotation;
    CharacterRotation = ShooterCharacter->GetActorRotation();
    const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
    const float Target = Delta.Yaw / DeltaTime;
    const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
    Lean = FMath::Clamp(Interp, -90.f, 90.f);

    AO_Yaw = ShooterCharacter->GetAO_Yaw();
    AO_Pitch = ShooterCharacter->GetAO_Pitch();
 
	// D(FString::Printf(TEXT("Base: %f"), AO_Yaw));
}
