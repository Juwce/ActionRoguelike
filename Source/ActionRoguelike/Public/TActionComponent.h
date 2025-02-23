// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TActionEffect.h"
#include "Components/ActorComponent.h"
#include "TActionComponent.generated.h"


class UTAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionStateChanged, UTActionComponent*, OwningComp, UTAction*, Action);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UTActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTActionComponent();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static UTActionComponent* GetActionComponent(const AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, const TSubclassOf<UTAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(UTAction* Action);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool HasAction(const TSubclassOf<UTAction> ActionClass) const;
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, const FName ActionName);
		
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, const FName ActionName);

	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStarted;
	
	UPROPERTY(BlueprintAssignable)
	FOnActionStateChanged OnActionStopped;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

protected:
	UFUNCTION(Server, Reliable)
	void ServerStartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(Server, Reliable)
	void ServerStopAction(AActor* Instigator, FName ActionName);

	// Default actions to add at BeginPlay()
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	TArray<TSubclassOf<UTAction>> DefaultActions;

	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<UTAction*> Actions;

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
};
