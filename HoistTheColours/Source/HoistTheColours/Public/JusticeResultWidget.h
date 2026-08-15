#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JusticeResultWidget.generated.h"

class UTextBlock;

UCLASS()
class HOISTTHECOLOURS_API UJusticeResultWidget
    : public UUserWidget
{
    GENERATED_BODY()

protected:

    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ResultText;
};