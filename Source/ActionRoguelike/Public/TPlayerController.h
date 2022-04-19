// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPlayerController.generated.h"

class ATPlayerState;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ATPlayerController*, OwningPlayer, float, NewCredits, float, Delta);

UCLASS()
class ACTIONROGUELIKE_API ATPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	ATPlayerState* GetTPlayerState() const;

	bool HasEnoughCredits(int32 Credits) const;
	
	// TODO: Move credits functionality into an interface and have this class inherit from it (we want non-player actors to be able to work with credits...)
	UFUNCTION(BlueprintCallable)
	void ApplyCreditChange(int32 Delta);

protected:
	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;
};

