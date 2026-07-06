#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MagnetComponent.generated.h"

UENUM(BlueprintType)
enum class EMagnetFalloff : uint8
{
	None        UMETA(DisplayName = "None"),
	Linear      UMETA(DisplayName = "Linear"),
	InverseSquare UMETA(DisplayName = "Inverse Square")
};

/**
 * 磁力系の処理をまとめるコンポーネント（USceneComponent ベース）。
 * - コンポーネントのワールド位置を基準に吸引/反発を処理できます。
 * - 物理オブジェクトには AddForce、キャラクターには簡易 Launch を適用。
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UMagnetComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UMagnetComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 有効化 / 無効化（親の SetActive と衝突しないよう名前を変更） */
	UFUNCTION(BlueprintCallable, Category = "Magnet")
	void SetMagnetActive(bool bOn);

	/** true = 反発、false = 吸引 */
	UFUNCTION(BlueprintCallable, Category = "Magnet")
	void SetRepel(bool bInRepel) { bRepel = bInRepel; }

	UFUNCTION(BlueprintCallable, Category = "Magnet")
	void SetStrength(float InStrength) { MagnetStrength = InStrength; }

	/** 半径（ワールド単位） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet")
	float Radius = 600.f;

	/** 力の強さ（基本スケール） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet")
	float MagnetStrength = 2000.f;

	/** 力の減衰 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet")
	EMagnetFalloff Falloff = EMagnetFalloff::InverseSquare;

	/** true = 反発（デフォルト）、false = 吸引 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet")
	bool bRepel = true;

	/** 物理オブジェクトに力を加えるか */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet")
	bool bAffectPhysics = true;

	/** キャラクター（ACharacter）にも影響させるか */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet")
	bool bAffectCharacters = true;

	/** どのコリジョンチャネルで検出するか */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet")
	TEnumAsByte<ECollisionChannel> QueryChannel = ECC_PhysicsBody;

	/** デバッグ描画を有効にするか */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet|Debug")
	bool bDrawDebug = false;

	/** デバッグ：描画表示時間（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet|Debug")
	float DebugDrawTime = 0.1f;

protected:
	/** 内部で有効か */
	bool bIsActive = true;

	/** 指定距離での力倍率を返す */
	float GetFalloffMultiplier(float Distance) const;

	/** 単フレームで力を適用する */
	void ApplyMagnetForces(float DeltaTime);
};