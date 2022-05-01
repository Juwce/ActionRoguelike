// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TActionComponent.generated.h"


class UTAction;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UTActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTActionComponent();

	UPROPERTY()
	TArray<UTAction*> Actions;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(const TSubclassOf<UTAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(const FName ActionName);
		
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(const FName ActionName);
};
