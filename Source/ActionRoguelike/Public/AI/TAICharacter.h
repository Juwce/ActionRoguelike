// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GameFramework/Character.h"
#include "TAICharacter.generated.h"

class UPawnSensingComponent;
UCLASS()
class ACTIONROGUELIKE_API ATAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATAICharacter();

protected:
	
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;
};
