// Simple enumeration of game phases used by GameState
#pragma once

#include "CoreMinimal.h"
#include "GamePhase.generated.h"

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	GP_Planning     UMETA(DisplayName = "Planning"),
	GP_ActionSubmit UMETA(DisplayName = "ActionSubmission"),
	GP_Resolution   UMETA(DisplayName = "Resolution"),
	GP_DayEnd       UMETA(DisplayName = "DayEnd")
};
