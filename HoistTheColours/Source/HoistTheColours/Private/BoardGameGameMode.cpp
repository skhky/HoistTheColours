#include "BoardGameGameMode.h"
#include "BoardGameGameState.h"
#include "BoardGamePlayerState.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

ABoardGameGameMode::ABoardGameGameMode()
{
    // Use our PlayerState class
    PlayerStateClass = ABoardGamePlayerState::StaticClass();
    GameStateClass = ABoardGameGameState::StaticClass();

    SecondsPerDay = 5.0f; // default for development: 5 seconds per in-game day

    for (int i = 0; i < 4; ++i) AssignedNations[i] = false;
}

void ABoardGameGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Initialize GameState day to 1 at start
    ABoardGameGameState* GS = GetGameState<ABoardGameGameState>();
    if (HasAuthority() && GS)
    {
        GS->CurrentDay = 1;
        GS->CurrentPhase = EGamePhase::GP_Planning;
        StartDayTimer();
    }
}

void ABoardGameGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (!HasAuthority()) return; // only server handles assignment

    ABoardGamePlayerState* PS = NewPlayer->GetPlayerState<ABoardGamePlayerState>();
    if (!PS) return;

    // Assign the lowest available NationId (0..3)
    int32 Assigned = -1;
    for (int i = 0; i < 4; ++i)
    {
        if (!AssignedNations[i])
        {
            Assigned = i;
            AssignedNations[i] = true;
            break;
        }
    }

    PS->NationId = Assigned;

    // Initialize player nation values here so they are replicated to clients
    // Why in GameMode: GameMode is server-authority and should set initial game state for players
    PS->Gold = 1000; // placeholder initial values, can be made editable later
    PS->Population = 1000;
    PS->ActionPoints = 3;
    PS->NationHP = 100;
    PS->bIsDefeated = false;

    UE_LOG(LogTemp, Log, TEXT("Player %s assigned Nation %d"), *PS->GetPlayerName(), PS->NationId);
}

void ABoardGameGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    // Free up assigned nation if any
    if (!HasAuthority()) return;
    APlayerState* PS = Exiting->PlayerState;
    if (PS)
    {
        ABoardGamePlayerState* BPS = Cast<ABoardGamePlayerState>(PS);
        if (BPS)
        {
            int32 N = BPS->NationId;
            if (N >= 0 && N < 4) AssignedNations[N] = false;
        }
    }
}

void ABoardGameGameMode::StartDayTimer()
{
    if (!HasAuthority()) return;
    if (SecondsPerDay <= 0.f) return;

    GetWorldTimerManager().SetTimer(DayTimerHandle, this, &ABoardGameGameMode::TickDay, SecondsPerDay, true);
}

void ABoardGameGameMode::StopDayTimer()
{
    if (!HasAuthority()) return;
    GetWorldTimerManager().ClearTimer(DayTimerHandle);
}

void ABoardGameGameMode::TickDay()
{
    if (!HasAuthority()) return;

    ABoardGameGameState* GS = GetGameState<ABoardGameGameState>();
    if (!GS) return;

    if (GS->CurrentDay >= GS->MaxDays)
    {
        UE_LOG(LogTemp, Log, TEXT("MaxDays reached (%d). Stopping day timer."), GS->MaxDays);
        StopDayTimer();
        return;
    }

    GS->CurrentDay += 1;

    UE_LOG(LogTemp, Log, TEXT("Day advanced: %d"), GS->CurrentDay);
}
