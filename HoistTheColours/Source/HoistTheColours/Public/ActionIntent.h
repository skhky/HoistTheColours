// ActionIntent.h
#pragma once

#include "CoreMinimal.h"
#include "EActionType.h"
#include "ActionIntent.generated.h"

USTRUCT(BlueprintType)
struct FActionIntent
{
    GENERATED_BODY()

    // Which action the player requests
    UPROPERTY()
    EActionType ActionType;

    // Target nation if applicable (-1 = none)
    UPROPERTY()
    int32 TargetNationId = -1;

    // Target tile id for tile-based actions (Investigation). -1 = none (use server auto-selection)
    UPROPERTY()
    int32 TargetTileId = -1;

    // Reserved for future minimal params; keep lightweight
    UPROPERTY()
    int64 GoldAmount = 0;
};
