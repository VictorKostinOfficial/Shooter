// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShooterGameModeBase.h"
#include "GameFramework/Character.h"

AShooterGameModeBase::AShooterGameModeBase()
{
}

void AShooterGameModeBase::StartPlay()
{
    Super::StartPlay();
}

void AShooterGameModeBase::OnActorKilled(AActor *VictimActor, AActor *Killer)
{
    ACharacter* Player = Cast<ACharacter>(VictimActor);

    if (Player)
    {
        FTimerHandle TimerHandle_RespawnDelay;
        FTimerDelegate Delegate;

        Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
        float RespawnDelay = 5.f;

        GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
    }

    UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void AShooterGameModeBase::RespawnPlayerElapsed(AController * Controller)
{
    if (ensure(Controller))
    {
        Controller->UnPossess();
        RestartPlayer(Controller);
    }
}

