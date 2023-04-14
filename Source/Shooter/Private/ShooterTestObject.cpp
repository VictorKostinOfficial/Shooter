#include "ShooterTestObject.h"


AShooterTestObject::AShooterTestObject()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
}

void AShooterTestObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShooterTestObject::Interact_Implementation(APawn *InstigatorPawn)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString(TEXT("Interact test object"))
		);
	}
}

void AShooterTestObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

