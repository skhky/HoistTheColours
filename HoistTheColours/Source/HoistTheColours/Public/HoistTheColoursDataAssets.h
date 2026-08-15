// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HoistTheColoursDataAssets.generated.h"


// ---------------------------------------------------------
// 選択肢
// ---------------------------------------------------------

USTRUCT(BlueprintType)
struct HOISTTHECOLOURS_API FJusticeChoice
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText ChoiceText;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true))
    FText ResultText;
};


// ---------------------------------------------------------
// プレイヤーごとの秘密情報
// ---------------------------------------------------------

USTRUCT(BlueprintType)
struct HOISTTHECOLOURS_API FJusticeSecret
{
    GENERATED_BODY()

public:

    // この秘密情報を持つプレイヤー番号
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 PlayerIndex = 0;

    // プレイヤーだけが見る情報
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true))
    FText SecretText;
};


// ---------------------------------------------------------
// シナリオ
// ---------------------------------------------------------

UCLASS(BlueprintType)
class HOISTTHECOLOURS_API UJusticeScenarioData : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:

    // シナリオタイトル
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText ScenarioTitle;

    // シナリオ説明
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true))
    FText ScenarioDescription;

    // 選択肢
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FJusticeChoice> Choices;

    // プレイヤーごとの秘密情報
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FJusticeSecret> Secrets;
};
