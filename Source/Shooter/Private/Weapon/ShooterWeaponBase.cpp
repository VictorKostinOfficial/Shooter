// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeaponBase.h"
#include "Components/SphereComponent.h"
#include "Weapon/ShooterWeaponComponent.h"
#include "Net/UnrealNetwork.h"

AShooterWeaponBase::AShooterWeaponBase()
{

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);

	LeftHandSocketName = "LeftHandSocket";

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


void AShooterWeaponBase::Interact_Implementation(APawn *InstigatorPawn)
{
	if (!ensure(InstigatorPawn))
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Green,FString::Printf(TEXT("ShooterWeaponBase interact")));

	UShooterWeaponComponent* WeaponComponent = UShooterWeaponComponent::GetWeaponComponent(InstigatorPawn);

	if(WeaponComponent->ApplyWeapon(this))
	{
		GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Green,FString::Printf(TEXT("ShooterWeaponBase Apply Weapon Branch")));

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
	}
}


void AShooterWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterWeaponBase, WeaponState);
}