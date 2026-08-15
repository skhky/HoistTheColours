#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HoistTheColoursPlayerController.generated.h"

// ========================================
// 前方宣言
// ========================================

class UJusticeSecretWidget;
class UJusticeVotingWidget;
class UJusticeResultWidget;


UCLASS()
class HOISTTHECOLOURS_API AHoistTheColoursPlayerController
    : public APlayerController
{
    GENERATED_BODY()

public:

    // ========================================
    // 正義の秘密
    // ========================================

    UFUNCTION(Client, Reliable)
    void Client_ShowSecret(
        const FText& SecretText
    );


    // ========================================
    // 正義の選択
    // ========================================

    UFUNCTION(Server, Reliable)
    void Server_SelectJustice(
        int32 ChoiceIndex
    );


    // ========================================
    // 投票UI
    // ========================================

    UFUNCTION(Client, Reliable)
    void Client_ShowVoting();


    // ========================================
    // 結果UI
    // ========================================

    UFUNCTION(Client, Reliable)
    void Client_ShowJusticeResult();


    // ========================================
    // 秘密UI
    // ========================================

    UPROPERTY(
        EditDefaultsOnly,
        Category = "Justice UI"
    )
    TSubclassOf<UJusticeSecretWidget>
        JusticeSecretWidgetClass;

    UPROPERTY()
    UJusticeSecretWidget*
        JusticeSecretWidget;


    // ========================================
    // 投票UI
    // ========================================

    UPROPERTY(
        EditDefaultsOnly,
        Category = "Justice UI"
    )
    TSubclassOf<UJusticeVotingWidget>
        JusticeVotingWidgetClass;

    UPROPERTY()
    UJusticeVotingWidget*
        JusticeVotingWidget;


    // ========================================
    // 結果UI
    // ========================================

    UPROPERTY(
        EditDefaultsOnly,
        Category = "Justice UI"
    )
    TSubclassOf<UJusticeResultWidget>
        JusticeResultWidgetClass;

    UPROPERTY()
    UJusticeResultWidget*
        JusticeResultWidget;
};