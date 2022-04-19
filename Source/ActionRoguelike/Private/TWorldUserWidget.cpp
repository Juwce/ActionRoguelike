// Fill out your copyright notice in the Description page of Project Settings.


#include "TWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

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
