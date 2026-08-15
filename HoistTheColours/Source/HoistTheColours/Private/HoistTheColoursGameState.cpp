#include "HoistTheColoursGameState.h"

#include "Net/UnrealNetwork.h"


void AHoistTheColoursGameState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps
) const
{
    Super::GetLifetimeReplicatedProps(
        OutLifetimeProps
    );


    // ========================================
    // CurrentPhaseを同期
    // ========================================

    DOREPLIFETIME(
        AHoistTheColoursGameState,
        CurrentPhase
    );


    // ========================================
    // WinningChoiceを同期
    // ========================================

    DOREPLIFETIME(
        AHoistTheColoursGameState,
        WinningChoice
    );
}