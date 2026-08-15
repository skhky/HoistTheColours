#include "HoistTheColoursPlayerState.h"
#include "Net/UnrealNetwork.h"

void AHoistTheColoursPlayerState::GetLifetimeReplicatedProps(
    TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AHoistTheColoursPlayerState, bHasVoted);
    DOREPLIFETIME(AHoistTheColoursPlayerState, SelectedChoice);
}