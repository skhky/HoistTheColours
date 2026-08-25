// BoardGameGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EActionType.h"
#include "ActionDefinition.h"
#include "ActionIntent.h"
#include "BoardGameGameMode.generated.h"

UCLASS()
class HOISTTHECOLOURS_API ABoardGameGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABoardGameGameMode();

    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;
    virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

    // Seconds per in-game day (for development/testing). Server drives day progression.
    UPROPERTY(EditDefaultsOnly, Category = "Game|Time")
    float SecondsPerDay;

    // Initialize action definitions
    void InitializeActionDefinitions();

    // Validate and execute an action intent. Returns true if action executed.
    bool ValidateAndExecuteAction(class ABoardGamePlayerState* PS, const struct FActionIntent& Intent, class AHoistTheColoursPlayerController* RequestPC);

protected:
    virtual void BeginPlay() override;

    void StartDayTimer();
    void StopDayTimer();
    void TickDay();

    FTimerHandle DayTimerHandle;

    // Keep track of assigned nations (0..3). true=assigned
    bool AssignedNations[4];

    // Action definitions map (ActionId -> definition). Minimal storage in GameMode for prototype.
    TMap<int32, struct FActionDefinition> ActionDefinitions;
};
