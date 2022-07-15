// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TActionComponent.h"
#include "Animation/AnimInstance.h"
#include "TAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UTAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UTActionComponent* ActionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned;
	
};
