// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TWorldUserWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UTWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	// Utility that handles the common need to create a widget if it does not already exist
	// ActiveWidgetRef will be populated with a new widget if null. 
	UFUNCTION(BlueprintCallable, Category = "UI")
	static bool ActivateWorldWidget(
		UTWorldUserWidget*& ActiveWidgetRef,
		const TSubclassOf<UTWorldUserWidget> WidgetClass,
		AActor* AttachedActor,
		const FVector OffsetFromActor);

	UFUNCTION()
	static bool ActivateWorldWidgetOnTimer(
		UTWorldUserWidget*& ActiveWidgetRef,
		const TSubclassOf<UTWorldUserWidget> WidgetClass,
		AActor* AttachedActor,
		const FVector OffsetFromActor,
		FTimerHandle& LifetimeHandle,
		const float LifetimeSeconds);
	
	// Removes widget from parent and nullifies ActiveWidgetRef
	UFUNCTION(BlueprintCallable, Category = "UI")
	static void DeactivateWorldWidget(UTWorldUserWidget*& ActiveWidgetRef);
	
	// Deactivate widget and clear timer
	UFUNCTION(BlueprintCallable, Category = "UI")
	static void DeactivateWorldWidgetAndTimer(UTWorldUserWidget*& ActiveWidgetRef, FTimerHandle& LifetimeHandle);

	void SetAttachedActor(AActor* Actor) { AttachedActor = Actor; };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta=(ExposeOnSpawn=true))
	FVector WorldOffset;

protected:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Translates the ParentSizeBox to the AttachedActor's location on the owning player's screen */
	void TranslateBoxToAttachedLocation();

	// Actor whose location this widget is attached to
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta=(ExposeOnSpawn=true))
	AActor* AttachedActor;

	// The SizeBox to translate to the player's screen
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

	FTimerHandle LifetimeTimerHandle;

};
