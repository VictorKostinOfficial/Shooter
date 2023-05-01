// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShooterActionComponent.generated.h"

class UShooterAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UShooterActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UShooterActionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddAction(TSubclassOf<UShooterAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopActionByName(AActor* Instigator, FName ActionName);


protected:

	// UFUNCTION(Server, Reliable)
	// void ServerStartAction(AActor* Instigator, FName ActionName);

	// UFUNCTION(Server, Reliable)
	// void ServerStopAction(AActor* Instigator, FName ActionName);

	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<UShooterAction>> DefaultActions;

	UPROPERTY(BlueprintReadOnly, Replicated)
	TArray<TObjectPtr<UShooterAction>> Actions;

	virtual void BeginPlay() override;

};
