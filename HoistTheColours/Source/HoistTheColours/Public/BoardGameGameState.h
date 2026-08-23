// BoardGameGameState.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GamePhase.h"
#include "BoardGameGameState.generated.h"

UCLASS()
class HOISTTHECOLOURS_API ABoardGameGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ABoardGameGameState();

    // Current in-game day (replicated to all clients)
    UPROPERTY(ReplicatedUsing=OnRep_CurrentDay)
    int32 CurrentDay;

    // Maximum days for the match (editable in defaults)
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    int32 MaxDays;

    // Current game phase (replicated)
    UPROPERTY(ReplicatedUsing=OnRep_CurrentPhase)
    EGamePhase CurrentPhase;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void OnRep_CurrentDay();

    UFUNCTION()
    void OnRep_CurrentPhase();
};
