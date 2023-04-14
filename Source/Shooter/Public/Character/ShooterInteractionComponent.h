// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterInteractionComponent.generated.h"

class UShooterWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterInteractionComponent : public UActorComponent
{

	GENERATED_BODY()

public:	
 
	void PrimaryInteract();

protected:

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	virtual void BeginPlay() override;

	void FindBestInteractable();
	
	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY()
	TObjectPtr<AActor> FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UShooterWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	TObjectPtr<UShooterWorldUserWidget> DefaultWidgetInstance;

public:

	UShooterInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};