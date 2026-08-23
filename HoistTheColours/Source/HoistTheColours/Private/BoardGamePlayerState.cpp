#include "BoardGamePlayerState.h"
#include "Net/UnrealNetwork.h"

ABoardGamePlayerState::ABoardGamePlayerState()
{
    NationId = -1;
    Gold = 1000; // default starting gold, editable later
    Population = 1000; // default
    ActionPoints = 3; // default AP
    NationHP = 100; // default HP
    bIsDefeated = false;
}

void ABoardGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABoardGamePlayerState, NationId);
    DOREPLIFETIME(ABoardGamePlayerState, Gold);
    DOREPLIFETIME(ABoardGamePlayerState, Population);
    DOREPLIFETIME(ABoardGamePlayerState, ActionPoints);
    DOREPLIFETIME(ABoardGamePlayerState, NationHP);
    DOREPLIFETIME(ABoardGamePlayerState, bIsDefeated);
}
