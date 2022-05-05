// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TAction.generated.h"

/**
 * 
 */
class UTActionComponent;

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API UTAction : public UObject
{
	GENERATED_BODY()

public:
	
	virtual UWorld* GetWorld() const override;

	void Initialize(UTActionComponent* OwningActionComponent);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* InstigatorActor);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Action")
	UTActionComponent* GetOwningComponent() const;

	/* Action Nickname to start/stop without a reference to the action instance */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintCallable)
	bool IsRunning() const { return bIsRunning; }

protected:

	UPROPERTY()
	UTActionComponent* OwningActionComp;

	/* Tags added to owning actor when activated, removed when action stops or completes */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	/* Action cannot start if OwningActor has any of these Tags applied */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags;
	
	bool bIsRunning;
	
};
