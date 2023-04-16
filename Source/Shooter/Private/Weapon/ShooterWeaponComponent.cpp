// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"


UShooterWeaponComponent::UShooterWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	SockedNameToAttach = "hand_rSocket";
	bIsAiming = false;

}


AActor *UShooterWeaponComponent::GetEquippedWeapon()
{
    return EquippedWeapon;
}


bool UShooterWeaponComponent::IsWeaponEquipped()
{
    return EquippedWeapon != nullptr;
}


UShooterWeaponComponent *UShooterWeaponComponent::GetWeaponComponent(AActor *FromActor)
{
	if (FromActor)
	{
		return Cast<UShooterWeaponComponent>(FromActor->GetComponentByClass(UShooterWeaponComponent::StaticClass()));
	}
    return nullptr;
}


void UShooterWeaponComponent::SetIsAiming(bool Value)
{
	bIsAiming = Value;
	Server_SetIsAiming(Value);
}


bool UShooterWeaponComponent::GetIsAiming()
{
	return bIsAiming;
}


bool UShooterWeaponComponent::ApplyWeapon(AActor *InstigatorActor, AActor *NewWeapon)
{
	
	GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Green,FString::Printf(TEXT("ShooterWeaponComponent ApplyWeapon")));
	
	if (GetOwner()->HasAuthority())
	{
		if (EquippedWeapon != NewWeapon) // TODO: Check by weapon type
		{
			EquippedWeapon = NewWeapon;	
			EquippedWeapon->SetOwner(InstigatorActor);
			EquippedWeapon->AttachToComponent(Cast<ACharacter>(InstigatorActor)->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SockedNameToAttach);
			Multicast_WeaponChanged(InstigatorActor, NewWeapon);
			return true;
		}	
	}
    return false;
}


void UShooterWeaponComponent::Server_SetIsAiming_Implementation(bool Value)
{
	bIsAiming = Value;
}


void UShooterWeaponComponent::Multicast_WeaponChanged_Implementation(AActor *InstigatorActor, AActor *NewWeapon)
{
	OnWeaponChanged.Broadcast(InstigatorActor, this, NewWeapon);
}


void UShooterWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UShooterWeaponComponent, EquippedWeapon);
	DOREPLIFETIME(UShooterWeaponComponent, bIsAiming);
}
