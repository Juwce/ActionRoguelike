// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPlayerController.generated.h"

class ATPlayerState;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);

UCLASS()
class ACTIONROGUELIKE_API ATPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintNativeEvent)
	void BlueprintBeginPlayingState();
	
	UFUNCTION(BlueprintCallable)
	ATPlayerState* GetTPlayerState() const;

	bool HasEnoughCredits(float Credits) const;
	
	// TODO: Move credits functionality into an interface and have this class inherit from it (we want non-player actors to be able to work with credits...)
	UFUNCTION(BlueprintCallable)
	void ApplyCreditChange(AActor* InstigatorActor, const float Delta);

protected:

	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged;

	virtual void SetPawn(APawn* InPawn) override;
};

