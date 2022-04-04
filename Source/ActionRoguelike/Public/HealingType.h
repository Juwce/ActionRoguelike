#pragma once

#include "CoreMinimal.h"
#include "HealingType.generated.h"

UENUM(BlueprintType)
enum class HealingType : uint8
{
	PercentOfHealthMax = 0,
	HealthPoints = 1
};