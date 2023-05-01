// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShooterAction.generated.h"

class UWorld;
class UShooterActionComponent;


USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	TObjectPtr<AActor> Instigator;

};


/**
 * 
 */
UCLASS()
class SHOOTER_API UShooterAction : public UObject
{
	GENERATED_BODY()
	
public:

	// void Initialize(UShooterActionComponent* NewActionComp);

	// UFUNCTION(BlueprintCallable, Category = "Action")
	// bool IsRunning();

	// UFUNCTION(BlueprintNativeEvent, Category = "Action")
	// bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	virtual UWorld* GetWorld() const override;

	// virtual bool IsSupportedForNetworking() const override
	// {
	// 	return true;
	// }

protected:

	// UPROPERTY(Replicated)
	// TObjectPtr<UShooterActionComponent> ActionComp;

	// UFUNCTION(BlueprintCallable, Category = "Action")
	// UShooterAction* GetOwningComponent() const;

	// UPROPERTY(ReplicatedUsing="OnRep_RepData")
	// FActionRepData RepData;

	// UPROPERTY(Replicated)
	// float TimeStarted;

	// UFUNCTION()
	// void OnRep_RepData();

	// void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
