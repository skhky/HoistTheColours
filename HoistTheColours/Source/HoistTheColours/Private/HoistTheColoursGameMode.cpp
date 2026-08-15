#include "HoistTheColoursGameMode.h"

#include "HoistTheColoursPlayerState.h"
#include "HoistTheColoursGameState.h"
#include "HoistTheColoursPlayerController.h"

#include "GameFramework/PlayerController.h"
#include "TimerManager.h"


AHoistTheColoursGameMode::AHoistTheColoursGameMode()
{
    // ========================================
    // 使用するPlayerStateを指定
    // ========================================

    PlayerStateClass =
        AHoistTheColoursPlayerState::StaticClass();


    // ========================================
    // 正義の秘密を登録
    // ========================================

    JusticeSecretTexts.Add(
        FText::FromString(
            TEXT("あなたは、社会全体の安全を守ることが最も重要な正義だと考えている。")
        )
    );

    JusticeSecretTexts.Add(
        FText::FromString(
            TEXT("あなたは、どんな状況でも一人ひとりの命を尊重することが正義だと考えている。")
        )
    );

    JusticeSecretTexts.Add(
        FText::FromString(
            TEXT("あなたは、悪を防ぐためなら多少の犠牲は必要悪として許されると考えている。")
        )
    );

    JusticeSecretTexts.Add(
        FText::FromString(
            TEXT("あなたは、法律に従うことこそが正義を守るために必要だと考えている。")
        )
    );
}


// ========================================
// プレイヤー参加
// ========================================

void AHoistTheColoursGameMode::PostLogin(
    APlayerController* NewPlayer
)
{
    Super::PostLogin(NewPlayer);

    if (!NewPlayer)
    {
        return;
    }

    // プレイヤーが参加したので秘密を配る
    AssignJusticeSecret(NewPlayer);
}


// ========================================
// ゲーム開始
// ========================================

void AHoistTheColoursGameMode::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("========================================")
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Hoist The Colours Game Start")
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("========================================")
    );

    StartDiscussionPhase();
}


// ========================================
// フェーズ変更
// ========================================

void AHoistTheColoursGameMode::SetJusticePhase(
    EJusticePhase NewPhase
)
{
    AHoistTheColoursGameState* GS =
        GetGameState<AHoistTheColoursGameState>();

    if (!GS)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("HoistTheColoursGameStateを取得できませんでした。")
        );

        return;
    }

    // フェーズを変更
    GS->CurrentPhase = NewPhase;

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Justice Phase Changed: %d"),
        static_cast<int32>(NewPhase)
    );
}


// ========================================
// Discussion
// ========================================

void AHoistTheColoursGameMode::StartDiscussionPhase()
{
    SetJusticePhase(
        EJusticePhase::Discussion
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Discussion Phase Started")
    );

    GetWorldTimerManager().SetTimer(
        PhaseTimerHandle,
        this,
        &AHoistTheColoursGameMode::StartVotingPhase,
        10.0f,
        false
    );
}


// ========================================
// Voting
// ========================================

void AHoistTheColoursGameMode::StartVotingPhase()
{
    SetJusticePhase(
        EJusticePhase::Voting
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Voting Phase Started")
    );


    // ========================================
    // 全プレイヤーに投票UIを表示
    // ========================================

    for (
        FConstPlayerControllerIterator It =
        GetWorld()->GetPlayerControllerIterator();
        It;
        ++It
        )
    {
        AHoistTheColoursPlayerController* PC =
            Cast<AHoistTheColoursPlayerController>(
                It->Get()
            );

        if (PC)
        {
            PC->Client_ShowVoting();
        }
    }
}


// ========================================
// 正義の選択確認
// ========================================

void AHoistTheColoursGameMode::CheckJusticeChoices()
{
    UWorld* World =
        GetWorld();

    if (!World)
    {
        return;
    }


    int32 PlayerCount = 0;

    int32 SelectedCount = 0;


    // ========================================
    // 全プレイヤーを確認
    // ========================================

    for (
        FConstPlayerControllerIterator It =
        World->GetPlayerControllerIterator();
        It;
        ++It
        )
    {
        AHoistTheColoursPlayerController* PC =
            Cast<AHoistTheColoursPlayerController>(
                It->Get()
            );

        if (!PC)
        {
            continue;
        }


        AHoistTheColoursPlayerState* PS =
            PC->GetPlayerState<
            AHoistTheColoursPlayerState
            >();

        if (!PS)
        {
            continue;
        }


        PlayerCount++;


        // ========================================
        // 選択済みか確認
        // ========================================

        if (
            PS->SelectedJusticeChoice >= 0 &&
            PS->SelectedJusticeChoice <= 2
            )
        {
            SelectedCount++;
        }
    }


    // ========================================
    // デバッグ
    // ========================================

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "Justice Choices : %d / %d"
        ),
        SelectedCount,
        PlayerCount
    );


    // ========================================
    // 全員選択完了
    // ========================================

    if (
        PlayerCount > 0 &&
        SelectedCount == PlayerCount
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
            TEXT("ALL PLAYERS SELECTED JUSTICE")
        );

        UE_LOG(
            LogTemp,
            Warning,
            TEXT("========================================")
        );


        // ========================================
        // 正義の結果を計算
        // ========================================

        CalculateJusticeResult();


        // ========================================
        // 全員に結果UIを表示
        // ========================================

        for (
            FConstPlayerControllerIterator It =
            World->GetPlayerControllerIterator();
            It;
            ++It
            )
        {
            AHoistTheColoursPlayerController* PC =
                Cast<AHoistTheColoursPlayerController>(
                    It->Get()
                );

            if (PC)
            {
                PC->Client_ShowJusticeResult();
            }
        }
    }
}


// ========================================
// 正義の結果を計算
// ========================================

void AHoistTheColoursGameMode::CalculateJusticeResult()
{
    int32 ChoiceCounts[3] =
    {
        0,
        0,
        0
    };


    UWorld* World =
        GetWorld();

    if (!World)
    {
        return;
    }


    // ========================================
    // 全プレイヤーの選択を集計
    // ========================================

    for (
        FConstPlayerControllerIterator It =
        World->GetPlayerControllerIterator();
        It;
        ++It
        )
    {
        AHoistTheColoursPlayerController* PC =
            Cast<AHoistTheColoursPlayerController>(
                It->Get()
            );

        if (!PC)
        {
            continue;
        }


        AHoistTheColoursPlayerState* PS =
            PC->GetPlayerState<
            AHoistTheColoursPlayerState
            >();

        if (!PS)
        {
            continue;
        }


        const int32 Choice =
            PS->SelectedJusticeChoice;


        if (
            Choice >= 0 &&
            Choice <= 2
            )
        {
            ChoiceCounts[Choice]++;
        }
    }


    // ========================================
    // 最多の選択肢を決定
    // ========================================

    int32 WinningChoice = 0;


    for (
        int32 i = 1;
        i < 3;
        ++i
        )
    {
        if (
            ChoiceCounts[i] >
            ChoiceCounts[WinningChoice]
            )
        {
            WinningChoice = i;
        }
    }


    // ========================================
    // GameStateに結果を保存
    // ========================================

    AHoistTheColoursGameState* GS =
        GetGameState<
        AHoistTheColoursGameState
        >();

    if (GS)
    {
        GS->WinningChoice =
            WinningChoice;
    }


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
        TEXT("Justice Result")
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Choice 0 : %d"),
        ChoiceCounts[0]
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Choice 1 : %d"),
        ChoiceCounts[1]
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Choice 2 : %d"),
        ChoiceCounts[2]
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Winning Choice : %d"),
        WinningChoice
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("========================================")
    );
}


// ========================================
// 正義の秘密を割り当てる
// ========================================

void AHoistTheColoursGameMode::AssignJusticeSecret(
    APlayerController* NewPlayer
)
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("===== AssignJusticeSecret START =====")
    );


    if (!NewPlayer)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("NewPlayer is NULL")
        );

        return;
    }


    UE_LOG(
        LogTemp,
        Warning,
        TEXT("NewPlayer exists")
    );


    // ========================================
    // PlayerControllerを取得
    // ========================================

    AHoistTheColoursPlayerController* PC =
        Cast<AHoistTheColoursPlayerController>(
            NewPlayer
        );


    if (!PC)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT(
                "HoistTheColoursPlayerController CAST FAILED"
            )
        );

        return;
    }


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "HoistTheColoursPlayerController CAST SUCCESS"
        )
    );


    // ========================================
    // 秘密が存在するか
    // ========================================

    if (
        JusticeSecretTexts.Num() == 0
        )
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("JusticeSecretTexts is EMPTY")
        );

        return;
    }


    // ========================================
    // 秘密を選択
    // ========================================

    const int32 SecretIndex =
        FMath::RandRange(
            0,
            JusticeSecretTexts.Num() - 1
        );


    const FText SecretText =
        JusticeSecretTexts[SecretIndex];


    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Selected secret index: %d"),
        SecretIndex
    );


    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Secret: %s"),
        *SecretText.ToString()
    );


    // ========================================
    // Client RPC
    // ========================================

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Calling Client_ShowSecret...")
    );


    PC->Client_ShowSecret(
        SecretText
    );


    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Client_ShowSecret called")
    );


    AssignedPlayerCount++;


    UE_LOG(
        LogTemp,
        Warning,
        TEXT("===== AssignJusticeSecret END =====")
    );
}