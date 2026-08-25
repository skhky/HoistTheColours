#include "HoistTheColoursPlayerController.h"
#include "BoardGameGameMode.h"
#include "BoardGamePlayerState.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"

void AHoistTheColoursPlayerController::Server_SubmitActionIntent_Implementation(const FActionIntent& Intent)
{
    // Server-side entry for client intents. Validate sender and forward to GameMode.
    ABoardGamePlayerState* PS = GetPlayerState<ABoardGamePlayerState>();
    if (!PS) return;

    ABoardGameGameMode* GM = GetWorld() ? Cast<ABoardGameGameMode>(GetWorld()->GetAuthGameMode()) : nullptr;
    if (!GM) return;

    // Delegate validation/execution to GameMode
    bool bOk = GM ? GM->ValidateAndExecuteAction(PS, Intent, this) : false;

    if (bOk)
    {
        Client_NotifyActionResult(true, TEXT("Action executed"));
    }
    else
    {
        Client_NotifyActionResult(false, TEXT("Action rejected"));
    }
}

void AHoistTheColoursPlayerController::Client_NotifyActionResult_Implementation(bool bSuccess, const FString& Reason)
{
    UE_LOG(LogTemp, Log, TEXT("ActionResult: %s - %s"), bSuccess ? TEXT("Success") : TEXT("Fail"), *Reason);
}
