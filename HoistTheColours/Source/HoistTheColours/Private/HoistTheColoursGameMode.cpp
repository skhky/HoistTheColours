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

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AHoistTheColoursPlayerController* PC =
            Cast<AHoistTheColoursPlayerController>(It->Get());

        if (PC)
        {
            PC->Client_ShowVoting();
        }
    }

    // 投票開始
    // ここではタイマーを設定しない
    //
    // 4人全員が投票したら
    // CheckVotes()
    // ↓
    // FinishVoting()
    // ↓
    // Result
}


// ========================================
// 投票確認
// ========================================

void AHoistTheColoursGameMode::CheckVotes()
{
    int32 VoteCounts[3] =
    {
        0,
        0,
        0
    };


    int32 VotedPlayerCount = 0;


    // ========================================
    // 全プレイヤーの投票を確認
    // ========================================

    for (APlayerState* PlayerState :
        GameState->PlayerArray)
    {
        AHoistTheColoursPlayerState* JusticePS =
            Cast<AHoistTheColoursPlayerState>(
                PlayerState
            );


        if (!JusticePS)
        {
            continue;
        }


        // 投票していなければ無視
        if (!JusticePS->bHasVoted)
        {
            continue;
        }


        // A / B / C
        if (
            JusticePS->SelectedChoice >= 0 &&
            JusticePS->SelectedChoice < 3
            )
        {
            VoteCounts[
                JusticePS->SelectedChoice
            ]++;

            VotedPlayerCount++;
        }
    }


    // ========================================
    // プレイヤー人数
    // ========================================

    const int32 PlayerCount =
        GameState->PlayerArray.Num();


    // ========================================
    // 全員投票していなければ終了しない
    // ========================================

    if (VotedPlayerCount < PlayerCount)
    {
        return;
    }


    // ========================================
    // 一番票が多い選択肢を探す
    // ========================================

    int32 WinningChoice = 0;


    for (int32 i = 1; i < 3; ++i)
    {
        if (
            VoteCounts[i] >
            VoteCounts[WinningChoice]
            )
        {
            WinningChoice = i;
        }
    }


    // ========================================
    // GameStateを取得
    // ========================================

    AHoistTheColoursGameState* GS =
        GetGameState<AHoistTheColoursGameState>();


    if (GS)
    {
        // 結果を保存
        GS->WinningChoice =
            WinningChoice;
    }


    // ========================================
    // デバッグログ
    // ========================================

    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "Voting Result: A=%d B=%d C=%d"
        ),
        VoteCounts[0],
        VoteCounts[1],
        VoteCounts[2]
    );


    UE_LOG(
        LogTemp,
        Warning,
        TEXT(
            "Winning Choice: %d"
        ),
        WinningChoice
    );


    // 投票終了
    FinishVoting();
}


// ========================================
// 投票終了
// ========================================

void AHoistTheColoursGameMode::FinishVoting()
{
    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Voting Finished")
    );


    // Resultフェーズへ
    StartResultPhase();
}


// ========================================
// Result
// ========================================

void AHoistTheColoursGameMode::StartResultPhase()
{
    SetJusticePhase(
        EJusticePhase::Result
    );


    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Result Phase Started")
    );


    // 現在はテスト用に10秒
    GetWorldTimerManager().SetTimer(
        PhaseTimerHandle,
        this,
        &AHoistTheColoursGameMode::StartDiscussionPhase,
        10.0f,
        false
    );
}

void AHoistTheColoursGameMode::CheckJusticeChoices()
{
    UWorld* World = GetWorld();

    if (!World)
    {
        return;
    }

    int32 PlayerCount = 0;
    int32 SelectedCount = 0;

    for (FConstPlayerControllerIterator It =
        World->GetPlayerControllerIterator();
        It;
        ++It)
    {
        AHoistTheColoursPlayerController* PC =
            Cast<AHoistTheColoursPlayerController>(It->Get());

        if (!PC)
        {
            continue;
        }

        AHoistTheColoursPlayerState* PS =
            PC->GetPlayerState<AHoistTheColoursPlayerState>();

        if (!PS)
        {
            continue;
        }

        PlayerCount++;

        if (PS->SelectedJusticeChoice >= 0 &&
            PS->SelectedJusticeChoice <= 2)
        {
            SelectedCount++;
        }
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Justice Choices : %d / %d"),
        SelectedCount,
        PlayerCount
    );

    // 全員選択完了
    if (PlayerCount > 0 &&
        SelectedCount == PlayerCount)
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
    }
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

    if (JusticeSecretTexts.Num() == 0)
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