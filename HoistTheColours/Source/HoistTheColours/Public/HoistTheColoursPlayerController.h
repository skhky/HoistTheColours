// HoistTheColoursPlayerController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ActionIntent.h"
#include "HoistTheColoursPlayerController.generated.h"

class ABoardGamePlayerState;

UCLASS()
class HOISTTHECOLOURS_API AHoistTheColoursPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    // Client -> Server: submit an action intent
    UFUNCTION(Server, Reliable)
    void Server_SubmitActionIntent(const FActionIntent& Intent);

    // Server -> Client: notify result
    UFUNCTION(Client, Reliable)
    void Client_NotifyActionResult(bool bSuccess, const FString& Reason);
};
