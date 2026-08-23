// BoardGameGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BoardGameGameMode.generated.h"

UCLASS()
class HOISTTHECOLOURS_API ABoardGameGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABoardGameGameMode();

    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;

    // Seconds per in-game day (for development/testing). Server drives day progression.
    UPROPERTY(EditDefaultsOnly, Category = "Game|Time")
    float SecondsPerDay;

protected:
    virtual void BeginPlay() override;

    void StartDayTimer();
    void StopDayTimer();
    void TickDay();

    FTimerHandle DayTimerHandle;

    // Keep track of assigned nations (0..3). true=assigned
    bool AssignedNations[4];
};
