// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TGameplayInterface.h"
#include "TItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ATItemChest : public AActor, public ITGameplayInterface
{
	GENERATED_BODY()

public:
	
	void Interact_Implementation(APawn* InstigatorPawn);

	void OnActorLoaded_Implementation() override;

	UPROPERTY(EditAnywhere)
	float TargetPitch;
	
public:	
	// Sets default values for this actor's properties
	ATItemChest();

protected:

	UPROPERTY(SaveGame, ReplicatedUsing="OnRep_bLidOpened", BlueprintReadOnly)
	bool bLidOpened;

	UFUNCTION()
	void OnRep_bLidOpened();
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;
};
