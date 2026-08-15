#include "HoistTheColoursPlayerController.h"
#include "HoistTheColoursPlayerState.h"
#include "HoistTheColoursGameState.h"
#include "HoistTheColoursGameMode.h"
#include "JusticeSecretWidget.h"

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

void AHoistTheColoursPlayerController::Server_Vote_Implementation(
    int32 ChoiceIndex
)
{
    // A / B / C以外は拒否
    if (
        ChoiceIndex < 0 ||
        ChoiceIndex > 2
        )
    {
        return;
    }


    // GameState取得
    AHoistTheColoursGameState* GS =
        GetWorld()->GetGameState<
        AHoistTheColoursGameState
        >();

    if (!GS)
    {
        return;
    }


    // Voting中以外は投票できない
    if (
        GS->CurrentPhase !=
        EJusticePhase::Voting
        )
    {
        return;
    }


    // PlayerState取得
    AHoistTheColoursPlayerState* PS =
        GetPlayerState<
        AHoistTheColoursPlayerState
        >();

    if (!PS)
    {
        return;
    }


    // 二重投票防止
    if (PS->bHasVoted)
    {
        return;
    }


    // 投票を保存
    PS->SelectedChoice =
        ChoiceIndex;

    PS->bHasVoted =
        true;


    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Player voted : %d"),
        ChoiceIndex
    );


    // GameMode取得
    AHoistTheColoursGameMode* GM =
        GetWorld()->GetAuthGameMode<
        AHoistTheColoursGameMode
        >();

    if (!GM)
    {
        return;
    }


    // 全員投票したか確認
    GM->CheckVotes();
}