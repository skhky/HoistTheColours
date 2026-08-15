#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HoistTheColoursGameState.h"
#include "HoistTheColoursGameMode.generated.h"

class APlayerController;

UCLASS()
class HOISTTHECOLOURS_API AHoistTheColoursGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:

    AHoistTheColoursGameMode();

    virtual void PostLogin(APlayerController* NewPlayer) override;

    // ========================================
    // 投票 (public so PlayerController can invoke CheckVotes)
    // ========================================

    void CheckVotes();

    void FinishVoting();

protected:

    virtual void BeginPlay() override;


    // ========================================
    // ゲームフェーズ
    // ========================================

    void SetJusticePhase(EJusticePhase NewPhase);

    void StartDiscussionPhase();

    void StartVotingPhase();

    void StartResultPhase();

public:
    void CheckJusticeChoices();

    // ========================================
    // タイマー
    // ========================================

    FTimerHandle PhaseTimerHandle;


    // ========================================
    // 正義の秘密
    // ========================================

    void AssignJusticeSecret(APlayerController* NewPlayer);


    // 4人分の秘密
    UPROPERTY()
    TArray<FText> JusticeSecretTexts;


    // 使用済み秘密番号
    UPROPERTY()
    TArray<int32> UsedSecretIndices;


    // 現在何人に秘密を配ったか
    int32 AssignedPlayerCount = 0;
};