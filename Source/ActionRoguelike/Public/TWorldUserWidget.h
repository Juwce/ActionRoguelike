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
	void SetAttachedActor(AActor* Actor) { AttachedActor = Actor; };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FVector WorldOffset;

protected:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Translates the ParentSizeBox to the AttachedActor's location on the owning player's screen */
	void TranslateBoxToAttachedLocation();

	// Actor whose location this widget is attached to
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AActor* AttachedActor;

	// The SizeBox to translate to the player's screen
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

};
