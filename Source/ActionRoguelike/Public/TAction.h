// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TAction.generated.h"

/**
 * 
 */
class UTActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UTAction : public UObject
{
	GENERATED_BODY()

public:

	UTAction();
	
	virtual UWorld* GetWorld() const override;

	void Initialize(UTActionComponent* OwningActionComponent);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* InstigatorActor);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable)
	bool IsRunning() const { return RepData.bIsRunning; }

	UFUNCTION(BlueprintCallable, Category = "Action")
	UTActionComponent* GetOwningComponent() const;

	/* Action Nickname to start/stop without a reference to the action instance */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	// Start immediately when added to the ActionComponent
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart;

protected:

	/* Tags added to owning actor when activated, removed when action stops or completes */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/* Action cannot start if OwningActor has any of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;
	
	UPROPERTY(Replicated)
	UTActionComponent* OwningActionComp;

	UPROPERTY(ReplicatedUsing="OnRep_IsRunning")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_IsRunning();

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	};
	
};
