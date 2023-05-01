// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShooterProjectileBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Character/ShooterAttributeComponent.h"

AShooterProjectileBase::AShooterProjectileBase()
{

	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	MoveComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMoveComponent");
	MoveComponent->bRotationFollowsVelocity = true;
	MoveComponent->bInitialVelocityInLocalSpace = true;
	MoveComponent->ProjectileGravityScale = 0.f;
	MoveComponent->InitialSpeed = 15000.f;

	bReplicates = false;
}


void AShooterProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (Tracer)
	{
		ParticleComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer,
			BoxComponent,
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition
		);
	}

	if (HasAuthority())
	{
		BoxComponent->OnComponentHit.AddDynamic(this, &AShooterProjectileBase::OnActorHit);
	}
}


void AShooterProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}


void AShooterProjectileBase::Explode_Implementation()
{
	// All hit effects
	if(ensure(IsValid(this)))	
	{
		if (ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
		}
		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
		}
		Destroy();
	}
}