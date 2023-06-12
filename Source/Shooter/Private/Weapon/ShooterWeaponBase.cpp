// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeaponBase.h"
#include "Components/SphereComponent.h"
#include "Weapon/ShooterWeaponComponent.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"

AShooterWeaponBase::AShooterWeaponBase()
{

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	FireDelay = 0.2f;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);

	LeftHandSocketName = "LeftHandSocket";
	MuzzleSocketName = "MuzzleFlash";

	// Set WorldDynamic obj type
	WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	// Ignor all collision responses
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

 	// Enabled to QueryOnly
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}


FName AShooterWeaponBase::GetSocketName_Implementation()
{
	return LeftHandSocketName;
}


USkeletalMeshComponent* AShooterWeaponBase::GetSkeletalMeshComponent_Implementation()
{
	return WeaponMesh;
}


EWeaponState AShooterWeaponBase::GetWeaponState_Implementation()
{
	return WeaponState;
}


void AShooterWeaponBase::Shoot_Implementation(const FVector& HitTarget)
{
	if (FireAnimation)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}
}

void AShooterWeaponBase::Drop_Implementation()
{
	WeaponState = EWeaponState::EWS_Dropped;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//WeaponMesh->DetachFromParent();
	//SetOwner(nullptr);
	// WeaponMesh->SetAllBodiesSimulatePhysics(true);
}


void AShooterWeaponBase::Interact_Implementation(APawn *InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	//GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Green,FString::Printf(TEXT("ShooterWeaponBase interact")));

	UShooterWeaponComponent* WeaponComponent = UShooterWeaponComponent::GetWeaponComponent(InstigatorPawn);

	if(WeaponComponent->ApplyWeapon(this))
	{
		//GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Green,FString::Printf(TEXT("ShooterWeaponBase Apply Weapon Branch")));

		WeaponState = EWeaponState::EWS_Equipped;
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void AShooterWeaponBase::OnRep_WeaponState()
{
	switch(WeaponState)
	{
		case EWeaponState::EWS_Equipped:
			WeaponState = EWeaponState::EWS_Equipped;
			WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case EWeaponState::EWS_Dropped:
			WeaponState = EWeaponState::EWS_Dropped;
			WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			break;
	}
}


void AShooterWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterWeaponBase, WeaponState);
}