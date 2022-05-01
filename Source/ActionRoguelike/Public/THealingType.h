#pragma once

#include "CoreMinimal.h"
#include "THealingType.generated.h"

UENUM(BlueprintType)
enum class EHealingType : uint8
{
	PercentOfHealthMax = 0,
	HealthPoints = 1
};