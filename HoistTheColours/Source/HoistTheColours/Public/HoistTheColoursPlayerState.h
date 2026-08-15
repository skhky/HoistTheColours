#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HoistTheColoursDataAssets.h"
#include "HoistTheColoursPlayerState.generated.h"

UCLASS()
class HOISTTHECOLOURS_API AHoistTheColoursPlayerState : public APlayerState
{
    GENERATED_BODY()

public:

    // このプレイヤーだけが持つ正義の秘密
    UPROPERTY(Replicated, BlueprintReadOnly)
    FJusticeSecret MyJusticeSecret;

    UPROPERTY(Replicated, BlueprintReadOnly)
    bool bHasVoted = false;

    UPROPERTY(Replicated, BlueprintReadOnly)
    int32 SelectedChoice = -1;

protected:

    virtual void GetLifetimeReplicatedProps(
        TArray<FLifetimeProperty>& OutLifetimeProps
    ) const override;
};
