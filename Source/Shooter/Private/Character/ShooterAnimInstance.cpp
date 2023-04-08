#include "Character/ShooterAnimInstance.h"

#include "Character/ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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


}
