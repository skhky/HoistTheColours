#include "JusticeSecretWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"


// ========================================
// 初期化
// ========================================

void UJusticeSecretWidget::NativeConstruct()
{
    Super::NativeConstruct();


    // ========================================
    // 確認ボタン
    // ========================================

    if (Button_Confirm)
    {
        Button_Confirm->OnClicked.AddDynamic(
            this,
            &UJusticeSecretWidget::OnConfirmClicked
        );
    }
}


// ========================================
// 秘密を設定
// ========================================

void UJusticeSecretWidget::SetSecretText(
    const FText& InSecretText
)
{
    if (!Text_Secret)
    {
        return;
    }


    Text_Secret->SetText(
        InSecretText
    );
}


// ========================================
// 確認ボタン
// ========================================

void UJusticeSecretWidget::OnConfirmClicked()
{
    RemoveFromParent();
}