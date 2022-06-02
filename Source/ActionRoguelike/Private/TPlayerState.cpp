// Fill out your copyright notice in the Description page of Project Settings.


#include "TPlayerState.h"

#include "Net/UnrealNetwork.h"

void ATPlayerState::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME( ATPlayerState, Credits );
}

