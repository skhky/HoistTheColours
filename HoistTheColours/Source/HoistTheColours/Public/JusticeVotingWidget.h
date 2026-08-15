#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JusticeVotingWidget.generated.h"

class UButton;
class AHoistTheColoursPlayerController;

UCLASS()
class HOISTTHECOLOURS_API UJusticeVotingWidget
    : public UUserWidget
{
    GENERATED_BODY()

protected:

    virtual void NativeConstruct() override;

    // ========================================
    // 選択肢ボタン
    // ========================================

    UPROPERTY(meta = (BindWidget))
    UButton* Button_01;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_02;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_03;


    // ========================================
    // ボタン処理
    // ========================================

    UFUNCTION()
    void OnButton01Clicked();

    UFUNCTION()
    void OnButton02Clicked();

    UFUNCTION()
    void OnButton03Clicked();


    // ========================================
    // 正義の選択を送信
    // ========================================

    void SelectJustice(int32 ChoiceIndex);
};