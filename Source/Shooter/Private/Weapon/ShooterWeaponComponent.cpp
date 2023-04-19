// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterWeaponComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x);}

UShooterWeaponComponent::UShooterWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	SockedNameToAttach = "hand_rSocket";
	bIsAiming = false;

	Character = Cast<ACharacter>(GetOwner());
	AimWalkSpeed = 450.f;

}


void UShooterWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	BaseWalkSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
}

AActor *UShooterWeaponComponent::GetEquippedWeapon()
{
    return EquippedWeapon;
}


bool UShooterWeaponComponent::IsWeaponEquipped()
{
    return EquippedWeapon != nullptr;
}


void UShooterWeaponComponent::AimOffset(float DeltaTime)
{
	if (!IsWeaponEquipped()) 
	{
		AimRotation = FRotator(0.f, Character->GetBaseAimRotation().Yaw, 0.f);
		return;
	}
	// D(FString::Printf(TEXT("Base: %f"), Base));
	// D(FString::Printf(TEXT("Camera: %f"), Camera));
	// D(FString::Printf(TEXT("AO_Yaw: %f"), AO_Yaw));
	// D(FString::Printf(TEXT("Weapon component: %s,  Equipped Weapon: %d"), WeaponComponent, !WeaponComponent->GetEquippedWeapon()));
    FVector Velocity = Character->GetVelocity();
    Velocity.Z = 0.f;
    float Speed = Velocity.Size();
	bool bIsInAir = Character->GetCharacterMovement()->IsFalling();
	float LocalYaw = 0;

	if (Speed == 0.f && !bIsInAir)
	{

		FRotator CurrentAimRotation = FRotator(0.f, Character->GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, AimRotation);

		Character->bUseControllerRotationYaw = false;
        LocalYaw = DeltaAimRotation.Yaw;

		//if(!Character->IsLocallyControlled())
		//{
			////D(TEXT("%f"))
			////D(TEXT("Current Aim Rotation:" + CurrentAimRotation.ToCompactString() + "\nAimRotation: " + AimRotation.ToCompactString() + "\nDelta Aim Rotation: " + DeltaAimRotation.ToCompactString() + " "));
			////D(FString::Printf(TEXT("Get Base Aim Rot Yaw: %f"), GetBaseAimRotation().Yaw));
			////D(FString::Printf(TEXT("First: %f"), AO_Yaw));
		//}
	}

	if (Speed > 0.f || bIsInAir)
	{
		AimRotation = FRotator(0.f, Character->GetBaseAimRotation().Yaw, 0.f);

		LocalYaw = 0.f;
		Character->bUseControllerRotationYaw = true;
		//D(FString::Printf(TEXT("SECOND: %f"), AO_Yaw));
	}

	// Yaw Rotation doesn't replicated by default with bUseControllerRotationYaw = false
	if (Character->IsLocallyControlled())
	{
		AO_Yaw = LocalYaw;
		Server_SetAO_Yaw(LocalYaw);
	}

	AO_Pitch = Character->GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !Character->IsLocallyControlled())	
	{
		AO_Pitch -= 360;
	}

    // GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, (FString::Printf(TEXT("Yaw: %f \n Pitch: %f"), AO_Yaw, AO_Pitch)));

}

void UShooterWeaponComponent::Server_SetAO_Yaw_Implementation(float AO_Yaw_New)
{
	AO_Yaw = AO_Yaw_New;
}


UShooterWeaponComponent *UShooterWeaponComponent::GetWeaponComponent(AActor *FromActor)
{
	if (FromActor)
	{
		return Cast<UShooterWeaponComponent>(FromActor->GetComponentByClass(UShooterWeaponComponent::StaticClass()));
	}
    return nullptr;
}


bool UShooterWeaponComponent::GetIsAiming()
{
	return bIsAiming;
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


void UShooterWeaponComponent::SetIsAiming(bool Value)
{
	bIsAiming = Value; // set value only on owning client
	Server_SetIsAiming(Value); // We need to make sure that value set on server
	if (Character != nullptr)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}


// void UShooterWeaponComponent::Server_EquippedWeapon_Implementation(AActor* NewWeapon)
// {
	// Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	// Character->bUseControllerRotationYaw = true;
// }


void UShooterWeaponComponent::Server_SetIsAiming_Implementation(bool Value)
{
	bIsAiming = Value;  // Set value on server and client

	if (Character != nullptr)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}


// void UShooterWeaponComponent::Multicast_WeaponChanged_Implementation(AActor *InstigatorActor, AActor *NewWeapon)
// {
	// OnWeaponChanged.Broadcast(InstigatorActor, this, NewWeapon);
// }


void UShooterWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UShooterWeaponComponent, EquippedWeapon);
	DOREPLIFETIME(UShooterWeaponComponent, bIsAiming);
	DOREPLIFETIME(UShooterWeaponComponent, AO_Yaw);
}
