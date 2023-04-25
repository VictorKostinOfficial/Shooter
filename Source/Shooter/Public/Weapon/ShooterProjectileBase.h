// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShooterProjectileBase.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UParticleSystem;
class USoundCue;

UCLASS(ABSTRACT)
class SHOOTER_API AShooterProjectileBase : public AActor
{
	GENERATED_BODY()
	
protected:

	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UParticleSystemComponent> ParticleComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MoveComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> Tracer;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> ImpactParticles;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<USoundCue> ImpactSound;

	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

public:	

	AShooterProjectileBase();

	virtual void BeginPlay() override;

};
