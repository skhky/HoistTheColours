#pragma once

#include "Engine/DataAsset.h"
#include "CharacterDataAsset.generated.h"

UCLASS(BlueprintType)
class HOISTTHECOLOURS_API UCharacterDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MaxHP = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MoveSpeed = 600.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float AttackPower = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText CharacterName;
};