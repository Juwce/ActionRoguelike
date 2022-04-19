// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ATPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	int32 GetCredits() const {
		return Credits;
	}

	void SetCredits(const float NewCredits)
	{
		Credits = NewCredits;
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Credits = 0;
};
