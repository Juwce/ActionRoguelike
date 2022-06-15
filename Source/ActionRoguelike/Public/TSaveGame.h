// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TSaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()
	
	// Identifier for which Actor this belongs to
	UPROPERTY()
	FString ActorName;
	
	// Store transform of movable actors
	UPROPERTY()
	FTransform Transform;

	// Store all SaveData properties on the actor
	UPROPERTY()
	TArray<uint8> ByteData;
};

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UTSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	int32 Credits;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
	
};
