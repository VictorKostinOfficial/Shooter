// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterFunctionLibrary.h"
#include "Character/ShooterAttributeComponent.h"

bool UShooterFunctionLibrary::ApplyDamage(AActor *DamageCauser, AActor *TargetActor, float DamageAmount)
{
	UShooterAttributeComponent* AttributeComponent = UShooterAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComponent)
	{
		return AttributeComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
    return false;
}

bool UShooterFunctionLibrary::ApplyDirectionalDamage(AActor *DamageCauser, AActor *TargetActor, float DamageAmount, const FHitResult &HitResult)
{
    if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
    	UE_LOG(LogTemp, Log, TEXT("Before If: %s"), *GetNameSafe(HitComp));
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
    		UE_LOG(LogTemp, Log, TEXT("ApplyDirectionalDamage: Victim: %s, Killer: %s"), *GetNameSafe(TargetActor), *GetNameSafe(DamageCauser));
			// Direction = Target - Origin
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			HitComp->AddImpulseAtLocation(Direction * 30000.f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}

	return false;
}
