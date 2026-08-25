// EActionType.h
#pragma once

#include "CoreMinimal.h"
#include "EActionType.generated.h"

// Minimal action types for testing the action system. Do not add game-specific actions here.
UENUM(BlueprintType)
enum class EActionType : uint8
{
    AT_None        UMETA(DisplayName = "None"),
    AT_Investigation UMETA(DisplayName = "Investigation (test)")
};
