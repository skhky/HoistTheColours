#include "BoardGameGameState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

ABoardGameGameState::ABoardGameGameState()
{
    CurrentDay = 0;
    MaxDays = 20; // default value, editable in editor
    CurrentPhase = EGamePhase::GP_Planning;
}

void ABoardGameGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABoardGameGameState, CurrentDay);
    DOREPLIFETIME(ABoardGameGameState, CurrentPhase);
}

void ABoardGameGameState::OnRep_CurrentDay()
{
    // This runs on clients when CurrentDay is updated on server.
    if (GEngine)
    {
        UE_LOG(LogTemp, Log, TEXT("OnRep_CurrentDay: %d"), CurrentDay);
    }
}

void ABoardGameGameState::OnRep_CurrentPhase()
{
    if (GEngine)
    {
        UE_LOG(LogTemp, Log, TEXT("OnRep_CurrentPhase: %d"), (uint8)CurrentPhase);
    }
}
