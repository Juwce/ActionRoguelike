// Fill out your copyright notice in the Description page of Project Settings.


#include "TPlayerState.h"

#include "Net/UnrealNetwork.h"

void ATPlayerState::SetCredits(AActor* InstigatorActor, const float NewCredits)
{
	ensure(InstigatorActor);
	
	if (HasAuthority())
	{
		const float Delta = NewCredits - Credits;
		Credits = NewCredits;
		MulticastCreditsChanged(InstigatorActor, Credits, Delta);
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Instigator [%s] attempted to change [%s]'s credits without authority!"),
			*GetNameSafe(InstigatorActor),
			*GetNameSafe(this));
	}
}

void ATPlayerState::LoadPlayerState_Implementation(const UTSaveGame* SaveGame)
{
	if (SaveGame)
	{
		Credits = SaveGame->Credits;
	}
}

void ATPlayerState::SavePlayerState_Implementation(UTSaveGame* SaveGame)
{
	SaveGame->Credits = GetCredits();
}

void ATPlayerState::MulticastCreditsChanged_Implementation(AActor* InstigatorActor, float NewCredits, float Delta)
{
	OnCreditsChanged.Broadcast(InstigatorActor, this, NewCredits, Delta);
}

void ATPlayerState::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME( ATPlayerState, Credits );
}
