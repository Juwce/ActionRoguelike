// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TInteractionComponent.generated.h"


class UTWorldUserWidget;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UTInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UTInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void PrimaryInteract();

protected:
	UFUNCTION(BlueprintCallable)
	bool ComputeInteractCandidates(TArray<FHitResult>& Hits);

	UFUNCTION(BlueprintCallable)
	AActor* SelectInteractionTarget();
	
	void UpdateInteractionWidgetAttachment(AActor* ActorToAttachTo);
	
	UPROPERTY(EditAnywhere)
	float MaxInteractDistance;

	// Widget to be displayed on the interactable object when the user is looking at it
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UTWorldUserWidget> InteractionWidgetClass;

	UPROPERTY()
	UTWorldUserWidget* InteractionWidgetInstance;
};
