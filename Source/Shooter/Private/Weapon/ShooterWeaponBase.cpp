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

	// Set WorldDynamic obj type
	WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	// Ignor all collision responses
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

 	// Enabled to QueryOnly
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

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

		WeaponState = EWeaponState2::EWS_Equipped;
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void AShooterWeaponBase::OnRep_WeaponState()
{
	switch(WeaponState)
	{
		case EWeaponState2::EWS_Equipped:
			WeaponState = EWeaponState2::EWS_Equipped;
			WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
	}
}

void AShooterWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterWeaponBase, WeaponState);
}