#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HoistTheColoursGameState.h"
#include "HoistTheColoursGameMode.generated.h"

class APlayerController;

UCLASS()
class HOISTTHECOLOURS_API AHoistTheColoursGameMode
    : public AGameModeBase
{
    GENERATED_BODY()

public:

    AHoistTheColoursGameMode();

    virtual void PostLogin(
        APlayerController* NewPlayer
    ) override;


    // ========================================
    // 正義の選択
    // ========================================

    // 全員が選択したか確認
    void CheckJusticeChoices();

    // 選択結果を集計
    void CalculateJusticeResult();


protected:

    virtual void BeginPlay() override;


    // ========================================
    // ゲームフェーズ
    // ========================================

    void SetJusticePhase(
        EJusticePhase NewPhase
    );

    void StartDiscussionPhase();

    void StartVotingPhase();


    // ========================================
    // タイマー
    // ========================================

    FTimerHandle PhaseTimerHandle;


    // ========================================
    // 正義の秘密
    // ========================================

    void AssignJusticeSecret(
        APlayerController* NewPlayer
    );


    // ========================================
    // 秘密一覧
    // ========================================

    UPROPERTY()
    TArray<FText> JusticeSecretTexts;


    // ========================================
    // 使用済み秘密番号
    // ========================================

    UPROPERTY()
    TArray<int32> UsedSecretIndices;


    // ========================================
    // 秘密を配った人数
    // ========================================

    int32 AssignedPlayerCount = 0;
};