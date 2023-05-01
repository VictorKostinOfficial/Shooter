// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/ShooterAction_Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x);}

UShooterAction_Projectile::UShooterAction_Projectile()
{
    StartPointSocketName = "MuzzleFlash";
    AttackAnimDelay = 0.1f;
}

void UShooterAction_Projectile::StartAction_Implementation(AActor *Instigator)
{
    Super::StartAction_Implementation(Instigator);

    ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		D(TEXT("StartAction_Implementation"));
		Character->PlayAnimMontage(AttackAnim);

        if (CastingEffect)
        {
		    UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), StartPointSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
        }

		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
	}
}

void UShooterAction_Projectile::AttackDelay_Elapsed(ACharacter *InstigatorCharacter)
{
    if (ensureAlways(ProjectileClass))
	{
		D(TEXT("AttackDelay_Elapsed"));
		FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(StartPointSocketName);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorCharacter;

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		// Ignore Player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();

		// endpoint far into the look-at distance (not too far, still adjust somewhat towards crosshair on a miss)
		FVector TraceEnd = TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000);

		FHitResult Hit;
		// returns true if we got to a blocking hit
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			// Overwrite trace end with impact point in world
			TraceEnd = Hit.ImpactPoint;
		}

		// find new direction/rotation from Hand pointing to impact point in world.
		FRotator ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction(InstigatorCharacter);
}
