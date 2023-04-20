// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeaponComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/ShooterWeaponInterface.h"

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x);}

UShooterWeaponComponent::UShooterWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	TurningInPlace = ETurningInPlace::ETIP_None;

	SockedNameToAttach = "hand_rSocket";
	bIsAiming = false;

	// Character = Cast<ACharacter>(GetOwner());
	AimWalkSpeed = 450.f;

}


void UShooterWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	BaseWalkSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
}


void UShooterWeaponComponent::AimOffset(float DeltaTime)
{
	if (!IsWeaponEquipped()) 
	{
		AimRotation = FRotator(0.f, Character->GetBaseAimRotation().Yaw, 0.f);
		return;
	}

    FVector Velocity = Character->GetVelocity();
    Velocity.Z = 0.f;
    float Speed = Velocity.Size();
	bool bIsInAir = Character->GetCharacterMovement()->IsFalling();
	float LocalYaw = 0;

	if (Speed == 0.f && !bIsInAir)
	{

		FRotator CurrentAimRotation = FRotator(0.f, Character->GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, AimRotation);

		// Character->bUseControllerRotationYaw = true;
		AO_Yaw = DeltaAimRotation.Yaw;
	
        // LocalYaw = DeltaAimRotation.Yaw;
		if (TurningInPlace == ETurningInPlace::ETIP_None)
		{
			InterpAO_Yaw = AO_Yaw;
		}

		TurnInPlace(DeltaTime);

	}

	if (Speed > 0.f || bIsInAir)
	{
		AimRotation = FRotator(0.f, Character->GetBaseAimRotation().Yaw, 0.f);

		// Character->bUseControllerRotationYaw = true;
		AO_Yaw = 0.f;

		// LocalYaw = 0.f;
		TurningInPlace = ETurningInPlace::ETIP_None;
	}

	// Yaw Rotation doesn't replicated by default with bUseControllerRotationYaw = false
	if (Character->IsLocallyControlled())
	{
		// AO_Yaw = LocalYaw;
		// Server_SetAO_Yaw(LocalYaw);
	}

	AO_Pitch = Character->GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !Character->IsLocallyControlled())	
	{
		AO_Pitch -= 360;
	}

}


void UShooterWeaponComponent::SetIsShooting(bool Value)
{
	bIsShooting = Value;
}


void UShooterWeaponComponent::TurnInPlace(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("AO_Yaw: %f"), AO_Yaw);
	if (AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}

	if (TurningInPlace != ETurningInPlace::ETIP_None)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);

		AO_Yaw = InterpAO_Yaw;

		if(FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_None;
			AimRotation = FRotator(0.f, Character->GetBaseAimRotation().Yaw, 0.f);
		}
	}
}


ETurningInPlace UShooterWeaponComponent::GetTurningInPlace()
{
    return TurningInPlace;
}


// void UShooterWeaponComponent::Server_SetAO_Yaw_Implementation(float AO_Yaw_New)
// {
// 	AO_Yaw = AO_Yaw_New;
// }


bool UShooterWeaponComponent::IsWeaponEquipped()
{
    return EquippedWeapon != nullptr;
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


bool UShooterWeaponComponent::ApplyWeapon(AActor *NewWeapon)
{
	GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Green,FString::Printf(TEXT("ShooterWeaponComponent ApplyWeapon")));
	
	if (GetOwner()->HasAuthority())
	{
		EquippedWeapon = NewWeapon;	
		EquippedWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SockedNameToAttach);
		EquippedWeapon->SetOwner(Character);

		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;

		// For Event
		// Server_EquippedWeapon(NewWeapon);
		// Multicast_WeaponChanged(InstigatorActor, NewWeapon);
		return true;
	}
    return false;
}


void UShooterWeaponComponent::OnRep_EquippedWeapon()
{
	if(EquippedWeapon != nullptr && Character != nullptr)
	{
		Character->GetCharacterMovement()->bOrientRotationToMovement = false;
		Character->bUseControllerRotationYaw = true;
	}

}


FName UShooterWeaponComponent::GetWeaponSocketName()
{
	//D("GetWeaponSocketName");
	if (EquippedWeapon == nullptr) return "";
	return IShooterWeaponInterface::Execute_GetSocketName(EquippedWeapon);
}


USkeletalMeshComponent* UShooterWeaponComponent::GetWeaponSkeletalMeshComponent()
{
	//D("GetWeaponSkeletalMeshComponent");
	if (EquippedWeapon == nullptr) return nullptr;
	return IShooterWeaponInterface::Execute_GetSkeletalMeshComponent(EquippedWeapon);
}


EWeaponState UShooterWeaponComponent::GetWeaponState()
{
	//D("GetWeaponState");
	if (EquippedWeapon == nullptr) return EWeaponState::EWS_None;
	return IShooterWeaponInterface::Execute_GetWeaponState(EquippedWeapon);

}


bool UShooterWeaponComponent::GetIsAiming()
{
	return bIsAiming;
}


void UShooterWeaponComponent::SetIsAiming(bool Value)
{
	bIsAiming = Value; // set value only on owning client
	Server_SetIsAiming(Value); // We need to make sure that value set on server
	if (Character != nullptr)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}


void UShooterWeaponComponent::Server_SetIsAiming_Implementation(bool Value)
{
	bIsAiming = Value;  // Set value on server and client

	if (Character != nullptr)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}


void UShooterWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UShooterWeaponComponent, EquippedWeapon);
	DOREPLIFETIME(UShooterWeaponComponent, bIsAiming);
	// DOREPLIFETIME(UShooterWeaponComponent, AO_Yaw);
}


// void UShooterWeaponComponent::Multicast_WeaponChanged_Implementation(AActor *InstigatorActor, AActor *NewWeapon)
// {
	// OnWeaponChanged.Broadcast(InstigatorActor, this, NewWeapon);
// }
