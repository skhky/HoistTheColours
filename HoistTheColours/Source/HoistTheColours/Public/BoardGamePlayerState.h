// BoardGamePlayerState.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BoardGamePlayerState.generated.h"

UCLASS()
class HOISTTHECOLOURS_API ABoardGamePlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    ABoardGamePlayerState();

    // Nation assigned to this player (0..3)
    UPROPERTY(Replicated)
    int32 NationId;

    UPROPERTY(Replicated)
    int64 Gold;

    UPROPERTY(Replicated)
    int32 Population;

    UPROPERTY(Replicated)
    int32 ActionPoints;

    UPROPERTY(Replicated)
    int32 NationHP;

    UPROPERTY(Replicated)
    bool bIsDefeated;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
