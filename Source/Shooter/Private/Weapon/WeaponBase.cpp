#include "Weapon/WeaponBase.h"
#include "Components/SphereComponent.h"
#include "Character/ShooterCharacter.h"
#include "Net/UnrealNetwork.h"

AWeaponBase::AWeaponBase()
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

void AWeaponBase::Interact_Implementation(APawn *InstigatorPawn)
{

	AShooterCharacter* InstigatorCharacter = Cast<AShooterCharacter>(InstigatorPawn);
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString(TEXT("" + InstigatorPawn->GetName() + ": Interact weapon"))
		);
	}

	//GEngine->AddOnScreenDebugMessage(-1,15.0f,FColor::Green,FString::Printf(TEXT("Find executed in %f secons."), stop-start));

	WeaponState = EWeaponState3::EWS_Equipped;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//InstigatorCharacter->Weapon = this;
	this->AttachToComponent(InstigatorCharacter->GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_rSocket"); 
}

void AWeaponBase::OnRep_WeaponState()
{
	switch(WeaponState)
	{
		case EWeaponState3::EWS_Equipped:
			WeaponState = EWeaponState3::EWS_Equipped;
			WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
	}
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponBase, WeaponState);
}