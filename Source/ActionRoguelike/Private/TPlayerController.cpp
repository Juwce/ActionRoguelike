// Fill out your copyright notice in the Description page of Project Settings.


#include "TPlayerController.h"

#include "TPlayerState.h"

ATPlayerState* ATPlayerController::GetTPlayerState() const
{
	if (!PlayerState)
	{
		return nullptr;
	}

	ATPlayerState* TPlayerState = Cast<ATPlayerState>(PlayerState);
	ensureMsgf(TPlayerState, TEXT("PlayerState is not of type TPlayerState! Please set the player state class in"
								  " the GameMode properties to be a subclass of TPlayerState."));
	return TPlayerState;
}

bool ATPlayerController::HasEnoughCredits(const int32 Credits) const
{
	ATPlayerState* TPlayerState = GetTPlayerState();
	if (TPlayerState)
	{
		return Credits <= TPlayerState->GetCredits();
	}
	
	return false;
}

void ATPlayerController::ApplyCreditChange(const int32 Delta)
{
	ATPlayerState* TPlayerState = GetTPlayerState();
	if (TPlayerState)
	{
		TPlayerState->SetCredits(TPlayerState->GetCredits() + Delta);
		OnCreditsChanged.Broadcast(this, TPlayerState->GetCredits(), Delta);
	}
}

void ATPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}
