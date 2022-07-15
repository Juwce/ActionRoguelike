// Fill out your copyright notice in the Description page of Project Settings.


#include "TAnimInstance.h"

#include "GameplayTagContainer.h"
#include "TActionComponent.h"

void UTAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	const AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = Cast<UTActionComponent>(OwningActor->GetComponentByClass(UTActionComponent::StaticClass()));
	}
}

void UTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
