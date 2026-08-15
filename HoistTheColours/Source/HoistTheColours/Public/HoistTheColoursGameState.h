#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HoistTheColoursGameState.generated.h"


UENUM(BlueprintType)
enum class EJusticePhase : uint8
{
    Lobby,
    Discussion,
    Voting,
    Result,
    Viewpoint,
    Completed
};

UCLASS()
class HOISTTHECOLOURS_API AHoistTheColoursGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    UPROPERTY(Replicated, BlueprintReadOnly)
    EJusticePhase CurrentPhase = EJusticePhase::Lobby;

    UPROPERTY(Replicated, BlueprintReadOnly)
    int32 WinningChoice = -1;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
