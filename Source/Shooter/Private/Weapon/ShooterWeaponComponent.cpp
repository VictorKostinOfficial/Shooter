// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


UShooterWeaponComponent::UShooterWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	SockedNameToAttach = "hand_rSocket";

}


AActor *UShooterWeaponComponent::GetEquippedWeapon()
{
    return EquippedWeapon;
}


UShooterWeaponComponent *UShooterWeaponComponent::GetWeaponComponent(AActor *FromActor)
{
	if (FromActor)
	{
		return Cast<UShooterWeaponComponent>(FromActor->GetComponentByClass(UShooterWeaponComponent::StaticClass()));
	}
    return nullptr;
}


bool UShooterWeaponComponent::ApplyWeapon(AActor *InstigatorActor, AActor* NewWeapon)
{
	
	GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Green,FString::Printf(TEXT("ShooterWeaponComponent ApplyWeapon")));
	
	if (GetOwner()->HasAuthority())
	{
		if (EquippedWeapon != NewWeapon) // TODO: Check by weapon type
		{
			EquippedWeapon = NewWeapon;	
			EquippedWeapon->AttachToComponent(Cast<ACharacter>(InstigatorActor)->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SockedNameToAttach);
			Multicast_WeaponChanged(InstigatorActor, NewWeapon);
			return true;
		}	
	}
    return false;
}


void UShooterWeaponComponent::Multicast_WeaponChanged_Implementation(AActor *InstigatorActor, AActor *NewWeapon)
{
	OnWeaponChanged.Broadcast(InstigatorActor, this, NewWeapon);
}


void UShooterWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UShooterWeaponComponent, EquippedWeapon);
}
