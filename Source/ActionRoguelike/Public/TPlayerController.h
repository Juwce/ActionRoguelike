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

protected:
	UFUNCTION(BlueprintCallable)
	void ApplyCreditChange(int32 Delta);

	// Spends specified number of credits if available, otherwise no credits are spend and this returns false.
	UFUNCTION(BlueprintCallable)
	bool SpendCredits(int32 Credits);

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;
};

