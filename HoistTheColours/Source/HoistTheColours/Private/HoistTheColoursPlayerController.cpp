#include "HoistTheColoursPlayerController.h"

#include "HoistTheColoursPlayerState.h"
#include "HoistTheColoursGameState.h"
#include "HoistTheColoursGameMode.h"

#include "JusticeSecretWidget.h"
#include "JusticeVotingWidget.h"
#include "JusticeResultWidget.h"


// ========================================
// 正義の秘密
// ========================================

void AHoistTheColoursPlayerController::Client_ShowSecret_Implementation(
    const FText& SecretText
)
{
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
    // Widgetが設定されているか
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
    // Widget作成
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
    // 表示
    // ========================================

    JusticeSecretWidget->AddToViewport();

    bShowMouseCursor = true;
}


// ========================================
// Voting UI表示
// ========================================

void AHoistTheColoursPlayerController::Client_ShowVoting_Implementation()
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Client_ShowVoting called")
    );


    // ========================================
    // Widget Class確認
    // ========================================

    if (!JusticeVotingWidgetClass)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "JusticeVotingWidgetClassが設定されていません。"
            )
        );

        return;
    }


    // ========================================
    // すでに表示されている場合
    // ========================================

    if (JusticeVotingWidget)
    {
        return;
    }


    // ========================================
    // Widget作成
    // ========================================

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
            TEXT(
                "JusticeVotingWidget作成失敗"
            )
        );

        return;
    }


    // ========================================
    // 表示
    // ========================================

    JusticeVotingWidget->AddToViewport();

    bShowMouseCursor = true;


    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Voting UI displayed")
    );
}


// ========================================
// 正義の選択
// ========================================

void AHoistTheColoursPlayerController::Server_SelectJustice_Implementation(
    int32 ChoiceIndex
)
{
    // ========================================
    // 0 / 1 / 2 以外は拒否
    // ========================================

    if (
        ChoiceIndex < 0 ||
        ChoiceIndex > 2
        )
    {
        return;
    }


    // ========================================
    // PlayerState取得
    // ========================================

    AHoistTheColoursPlayerState* PS =
        GetPlayerState<
        AHoistTheColoursPlayerState
        >();


    if (!PS)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "HoistTheColoursPlayerState取得失敗"
            )
        );

        return;
    }


    // ========================================
    // 二重選択防止
    // ========================================

    if (
        PS->SelectedJusticeChoice != -1
        )
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT(
                "Player already selected justice."
            )
        );

        return;
    }


    // ========================================
    // 選択を保存
    // ========================================

    PS->SelectedJusticeChoice =
        ChoiceIndex;


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "Player selected justice choice : %d"
        ),
        ChoiceIndex
    );


    // ========================================
    // GameMode取得
    // ========================================

    AHoistTheColoursGameMode* GM =
        GetWorld()->GetAuthGameMode<
        AHoistTheColoursGameMode
        >();


    if (!GM)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "HoistTheColoursGameMode取得失敗"
            )
        );

        return;
    }


    // ========================================
    // 全員選択済みか確認
    // ========================================

    GM->CheckJusticeChoices();
}


// ========================================
// 結果UI表示
// ========================================

void AHoistTheColoursPlayerController::Client_ShowJusticeResult_Implementation()
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Client_ShowJusticeResult called")
    );


    // ========================================
    // Widget Class確認
    // ========================================

    if (!JusticeResultWidgetClass)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "JusticeResultWidgetClassが設定されていません。"
            )
        );

        return;
    }


    // ========================================
    // すでに表示されている場合
    // ========================================

    if (JusticeResultWidget)
    {
        return;
    }


    // ========================================
    // Widget作成
    // ========================================

    JusticeResultWidget =
        CreateWidget<UJusticeResultWidget>(
            this,
            JusticeResultWidgetClass
        );


    if (!JusticeResultWidget)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "JusticeResultWidget作成失敗"
            )
        );

        return;
    }


    // ========================================
    // 表示
    // ========================================

    JusticeResultWidget->AddToViewport();

    bShowMouseCursor = true;


    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Justice Result UI displayed")
    );
}