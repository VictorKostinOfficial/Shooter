// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ShooterAttributeComponent.h"
#include "Net/UnrealNetwork.h"

#define D(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, x);}

UShooterAttributeComponent::UShooterAttributeComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

	HealthMax = 100;
	Health = HealthMax;

	SetIsReplicatedByDefault(true);

}


UShooterAttributeComponent *UShooterAttributeComponent::GetAttributes(AActor *FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<UShooterAttributeComponent>();
	}
    return nullptr;
}


bool UShooterAttributeComponent::IsActorAlive(AActor *Actor)
{
	UShooterAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if (AttributeComponent)
	{
		return AttributeComponent->IsAlive();
	}
    return false;
}


float UShooterAttributeComponent::GetHealthMax() const
{
    return HealthMax;
}


float UShooterAttributeComponent::GetHealth() const
{
    return Health;
}

bool UShooterAttributeComponent::IsAlive() const
{
    return Health > 0.f;
}

bool UShooterAttributeComponent::IsFullHealth() const
{
    return Health == HealthMax;
}

bool UShooterAttributeComponent::ApplyHealthChange(AActor *InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.f)
	{
		return false;
	}
	   
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;

	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != 0.f)
		{
			Multicast_HealthChanged(InstigatorActor, Health, ActualDelta);
			D(FString::Printf(TEXT("Current Health: %f"), NewHealth));
		}

		if (ActualDelta < 0.f && Health == 0.f)
		{
			// GetOwner()->Destroy();
			D(TEXT("KILLED"));
		}
	}

    return ActualDelta != 0.f;
}

void UShooterAttributeComponent::Multicast_HealthChanged_Implementation(AActor *InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void UShooterAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UShooterAttributeComponent, HealthMax);
	DOREPLIFETIME(UShooterAttributeComponent, Health);
}