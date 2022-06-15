// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TGameModeBase.h"
#include "TSaveGame.h"
#include "GameFramework/PlayerState.h"
#include "TPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCreditsChanged, AActor*, Instigator, ATPlayerState*, OwningPlayerState, float, NewCredits, float, Delta);
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ATPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	float GetCredits() const { return Credits; }

	UFUNCTION(BlueprintCallable)
	void SetCredits(AActor* InstigatorActor, const float NewCredits);

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(UTSaveGame* SaveGame);
	
	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(const UTSaveGame* SaveGame);

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	float Credits = 0;
	
	UFUNCTION(NetMulticast, Reliable) // TODO: mark as unreliable once we move the 'state' out of TCharacter
	void MulticastCreditsChanged(AActor* InstigatorActor, float NewCredits, float Delta);

	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;
};
