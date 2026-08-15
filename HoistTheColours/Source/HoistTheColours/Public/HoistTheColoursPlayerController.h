#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HoistTheColoursPlayerController.generated.h"

UCLASS()
class HOISTTHECOLOURS_API AHoistTheColoursPlayerController
    : public APlayerController
{
    GENERATED_BODY()

public:

    // ========================================
    // サーバーから本人だけに秘密を送る
    // ========================================

    UFUNCTION(Client, Reliable)
    void Client_ShowSecret(
        const FText& SecretText
    );


    // ========================================
    // プレイヤーの投票
    // ========================================

    UFUNCTION(Server, Reliable)
    void Server_SelectJustice(
        int32 ChoiceIndex
    );

    UFUNCTION(Client, Reliable)
    void Client_ShowVoting();

    // ========================================
    // UI
    // ========================================
    UPROPERTY(EditDefaultsOnly, Category = "Justice UI")
    TSubclassOf<class UJusticeSecretWidget> JusticeSecretWidgetClass;

    UPROPERTY()
    class UJusticeSecretWidget* JusticeSecretWidget;


    UPROPERTY(EditDefaultsOnly, Category = "Justice UI")
    TSubclassOf<class UJusticeVotingWidget> JusticeVotingWidgetClass;

    UPROPERTY()
    class UJusticeVotingWidget* JusticeVotingWidget;
};