// Fill out your copyright notice in the Description page of Project Settings.


#include "TWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"


bool UTWorldUserWidget::ActivateWorldWidget(UTWorldUserWidget*& ActiveWidgetRef,
                                                    const TSubclassOf<UTWorldUserWidget> WidgetClass,
                                                    AActor* AttachedActor,
                                                    const FVector OffsetFromActor)
{
	if (!ensure(AttachedActor) || !ensure(WidgetClass))
	{
		return false;
	}
	
	if (IsValid(ActiveWidgetRef))
	{
		return true;
	}
	
	ActiveWidgetRef = CreateWidget<UTWorldUserWidget>(AttachedActor->GetWorld(), WidgetClass);
	if (ensure(ActiveWidgetRef))
	{
		ActiveWidgetRef->WorldOffset = OffsetFromActor;
		ActiveWidgetRef->SetAttachedActor(AttachedActor);
		ActiveWidgetRef->AddToViewport();
	}

	// something went wrong
	return false;
}

bool UTWorldUserWidget::ActivateWorldWidgetOnTimer(UTWorldUserWidget*& ActiveWidgetRef,
	const TSubclassOf<UTWorldUserWidget> WidgetClass, AActor* AttachedActor, const FVector OffsetFromActor,
	FTimerHandle& LifetimeHandle, const float LifetimeSeconds)
{
	bool bSuccess = false;
	if (ensure(AttachedActor))
	{
		FTimerDelegate TimerDel;
		TimerDel.BindLambda([&ActiveWidgetRef]
		{
			DeactivateWorldWidget(ActiveWidgetRef);
		});
		AttachedActor->GetWorld()->GetTimerManager().ClearTimer(LifetimeHandle);
		AttachedActor->GetWorld()->GetTimerManager().SetTimer(LifetimeHandle, TimerDel, LifetimeSeconds, false);
		bSuccess = ActivateWorldWidget(ActiveWidgetRef, WidgetClass, AttachedActor, OffsetFromActor);
	}
	
	return bSuccess;
}


void UTWorldUserWidget::DeactivateWorldWidget(UTWorldUserWidget*& ActiveWidgetRef)
{
	if (IsValid(ActiveWidgetRef))
	{
		ActiveWidgetRef->RemoveFromParent();
		ActiveWidgetRef = nullptr;
	}
}

void UTWorldUserWidget::DeactivateWorldWidgetAndTimer(UTWorldUserWidget*& ActiveWidgetRef, FTimerHandle& LifetimeHandle)
{
	if (IsValid(ActiveWidgetRef))
	{
		ActiveWidgetRef->GetWorld()->GetTimerManager().ClearTimer(LifetimeHandle);
	}
	
	DeactivateWorldWidget(ActiveWidgetRef);
}


void UTWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TranslateBoxToAttachedLocation();
}


void UTWorldUserWidget::TranslateBoxToAttachedLocation()
{
	if (!ensure(IsValid(AttachedActor)))
	{
		RemoveFromParent();
		UE_LOG(LogTemp, Warning, TEXT("AttachedActor no longer valid, removing Health Widget."));
		return;
	}

	if (!ensureMsgf(ParentSizeBox, TEXT("ParentSizeBox has not been bound. Create a variable in the UI designer with a"
		                " name and type identical to this variable to have Unreal set it for you.")))
	{
		return;
	}
	
	FVector2D ScreenPosition;
	const bool bWorldProjectionSucceeded = UGameplayStatics::ProjectWorldToScreen(
		GetOwningPlayer(),
		AttachedActor->GetActorLocation() + WorldOffset,
		ScreenPosition);

	if (bWorldProjectionSucceeded)
	{
		const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
		ParentSizeBox->SetRenderTranslation(ScreenPosition / ViewportScale);
	}
}
