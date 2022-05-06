// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TActionEffect.h"
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

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UTActionComponent* GetActionComponent(const AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, const TSubclassOf<UTAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(UTAction* Action);
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, const FName ActionName);
		
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, const FName ActionName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	// Default actions to add at BeginPlay()
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TArray<TSubclassOf<UTAction>> DefaultActions;

	UPROPERTY()
	TArray<UTAction*> Actions;
};
