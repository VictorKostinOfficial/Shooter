// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterProjectile_Dash.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundCue.h"

AShooterProjectile_Dash::AShooterProjectile_Dash()
{
    TeleportDelay = 0.2f;
    DetonateDelay = 0.2f;

    MoveComponent->InitialSpeed = 6000.f;
}

void AShooterProjectile_Dash::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &AShooterProjectile_Dash::Explode, DetonateDelay);
}

void AShooterProjectile_Dash::Explode_Implementation()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

    if (ImpactParticles)
        UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticles, GetActorLocation(), GetActorRotation());

    if (ImpactSound)     
        UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, GetActorLocation());

    // ParticleComponent->DeactivateSystem();

    MoveComponent->StopMovementImmediately();
    SetActorEnableCollision(false);

    FTimerHandle TimerHandle_DelayedTeleport;
    GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &AShooterProjectile_Dash::TeleportInstigator, TeleportDelay);

}

void AShooterProjectile_Dash::TeleportInstigator()
{
    AActor* ActorToTeleport = GetInstigator();
    if(ensure(ActorToTeleport))
    {
        ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, true);

        APawn* InstigatorPawn = Cast<APawn>(ActorToTeleport);
        APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
    }

    Destroy();
}

