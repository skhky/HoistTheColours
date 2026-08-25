// ActionDefinition.h
#pragma once

#include "CoreMinimal.h"
#include "EActionType.h"
#include "ActionDefinition.generated.h"

USTRUCT()
struct FActionDefinition
{
    GENERATED_BODY()

    UPROPERTY()
    EActionType ActionId = EActionType::AT_None;

    UPROPERTY()
    FName ActionName = NAME_None;

    // Action Points cost for this action
    UPROPERTY()
    int32 ActionPointCost = 1;

    // DurationDays that this action consumes; MUST be used by GameMode when advancing days
    UPROPERTY()
    int32 DurationDays = 10; // default for prototype; per-action configurable
};
