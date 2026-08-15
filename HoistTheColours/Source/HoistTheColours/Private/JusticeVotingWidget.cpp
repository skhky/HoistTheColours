#include "JusticeVotingWidget.h"

#include "Components/Button.h"
#include "HoistTheColoursPlayerController.h"

void UJusticeVotingWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // ========================================
    // ボタンイベント登録
    // ========================================

    if (Button_01)
    {
        Button_01->OnClicked.AddDynamic(
            this,
            &UJusticeVotingWidget::OnButton01Clicked
        );
    }

    if (Button_02)
    {
        Button_02->OnClicked.AddDynamic(
            this,
            &UJusticeVotingWidget::OnButton02Clicked
        );
    }

    if (Button_03)
    {
        Button_03->OnClicked.AddDynamic(
            this,
            &UJusticeVotingWidget::OnButton03Clicked
        );
    }
}


// ========================================
// Button 01
// ========================================

void UJusticeVotingWidget::OnButton01Clicked()
{
    SelectJustice(0);
}


// ========================================
// Button 02
// ========================================

void UJusticeVotingWidget::OnButton02Clicked()
{
    SelectJustice(1);
}


// ========================================
// Button 03
// ========================================

void UJusticeVotingWidget::OnButton03Clicked()
{
    SelectJustice(2);
}


// ========================================
// 正義の選択
// ========================================

void UJusticeVotingWidget::SelectJustice(
    int32 ChoiceIndex
)
{
    AHoistTheColoursPlayerController* PC =
        GetOwningPlayer<
        AHoistTheColoursPlayerController
        >();

    if (!PC)
    {
        UE_LOG(
            LogTemp,
            Error,
            TEXT("JusticeVotingWidget: PlayerController取得失敗")
        );

        return;
    }

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Justice Choice Selected : %d"),
        ChoiceIndex
    );

    PC->Server_SelectJustice(
        ChoiceIndex
    );

    RemoveFromParent();
}