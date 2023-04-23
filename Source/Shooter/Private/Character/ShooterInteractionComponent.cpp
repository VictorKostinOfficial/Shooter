#include "Character/ShooterInteractionComponent.h"
#include "ShooterInteractionInterface.h"
#include "HUD/ShooterWorldUserWidget.h"
#include "DrawDebugHelpers.h"

UShooterInteractionComponent::UShooterInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostUpdateWork;

	TraceDistance = 600;
	TraceRadius = 50;
	CollisionChannel = ECollisionChannel::ECC_Visibility;
}

void UShooterInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UShooterInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void UShooterInteractionComponent::FindBestInteractable()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * TraceDistance);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	bool bBlockHit = GetWorld()->SweepMultiByChannel(Hits, EyeLocation, End, FQuat::Identity, CollisionChannel, Shape, Params);
	FColor LineColor = bBlockHit ? FColor::Green : FColor::Red;

	FocusedActor = nullptr;

	for (FHitResult Hit : Hits)
	{
		// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f);
		
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UShooterInteractionInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UShooterWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	// DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}

void UShooterInteractionComponent::ServerInteract_Implementation(AActor *InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());
	IShooterInteractionInterface::Execute_Interact(InFocus, MyPawn);
}

void UShooterInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

