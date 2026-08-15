#include "HoistTheColoursPlayerController.h"
#include "HoistTheColoursPlayerState.h"
#include "HoistTheColoursGameState.h"
#include "HoistTheColoursGameMode.h"
#include "JusticeSecretWidget.h"
#include "JusticeVotingWidget.h"

// ========================================
// 秘密情報
// ========================================

void AHoistTheColoursPlayerController::Client_ShowSecret_Implementation(
    const FText& SecretText
)
{
    // ========================================
    // デバッグログ
    // ========================================

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("========================================")
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("あなたの正義の秘密")
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("%s"),
        *SecretText.ToString()
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("========================================")
    );


    // ========================================
    // Widgetが設定されているか確認
    // ========================================

    if (!JusticeSecretWidgetClass)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "JusticeSecretWidgetClassが設定されていません。"
            )
        );

        return;
    }


    // ========================================
    // Widgetを作成
    // ========================================

    JusticeSecretWidget =
        CreateWidget<UJusticeSecretWidget>(
            this,
            JusticeSecretWidgetClass
        );


    if (!JusticeSecretWidget)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "JusticeSecretWidgetを作成できませんでした。"
            )
        );

        return;
    }


    // ========================================
    // 秘密を設定
    // ========================================

    JusticeSecretWidget->SetSecretText(
        SecretText
    );


    // ========================================
    // 画面に表示
    // ========================================

    JusticeSecretWidget->AddToViewport();


    // ========================================
    // UI操作を可能にする
    // ========================================

    bShowMouseCursor = true;
}
// ========================================
// 投票
// ========================================

void AHoistTheColoursPlayerController::Server_SelectJustice_Implementation(
    int32 ChoiceIndex
)
{
    if (ChoiceIndex < 0 || ChoiceIndex > 2)
    {
        return;
    }

    AHoistTheColoursPlayerState* PS =
        GetPlayerState<AHoistTheColoursPlayerState>();

    if (!PS)
    {
        return;
    }

    // 二重選択防止
    if (PS->SelectedJusticeChoice != -1)
    {
        return;
    }

    PS->SelectedJusticeChoice = ChoiceIndex;

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Player selected justice choice : %d"),
        ChoiceIndex
    );

    AHoistTheColoursGameMode* GM =
        GetWorld()->GetAuthGameMode<
        AHoistTheColoursGameMode
        >();

    if (GM)
    {
        GM->CheckJusticeChoices();
    }
}

void AHoistTheColoursPlayerController::Client_ShowVoting_Implementation()
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Client_ShowVoting called")
    );

    if (!JusticeVotingWidgetClass)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("JusticeVotingWidgetClassが設定されていません。")
        );

        return;
    }

    if (JusticeVotingWidget)
    {
        return;
    }

    JusticeVotingWidget =
        CreateWidget<UJusticeVotingWidget>(
            this,
            JusticeVotingWidgetClass
        );

    if (!JusticeVotingWidget)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("JusticeVotingWidget作成失敗")
        );

        return;
    }

    JusticeVotingWidget->AddToViewport();

    bShowMouseCursor = true;

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Voting UI displayed")
    );
}