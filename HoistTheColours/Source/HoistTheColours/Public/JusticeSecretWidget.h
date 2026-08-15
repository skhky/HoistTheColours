#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JusticeSecretWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class HOISTTHECOLOURS_API UJusticeSecretWidget
    : public UUserWidget
{
    GENERATED_BODY()

public:

    // ========================================
    // 秘密を表示
    // ========================================

    void SetSecretText(
        const FText& InSecretText
    );


protected:

    // ========================================
    // 秘密本文
    // ========================================

    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Secret;


    // ========================================
    // 確認ボタン
    // ========================================

    UPROPERTY(meta = (BindWidget))
    UButton* Button_Confirm;


    // ========================================
    // 確認ボタンが押された
    // ========================================

    UFUNCTION()
    void OnConfirmClicked();


    virtual void NativeConstruct() override;
};