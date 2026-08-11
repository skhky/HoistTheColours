#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Engine/EngineTypes.h"
#include "MagnetComponent.generated.h"

class AActor;
class ACharacter;

UENUM(BlueprintType)
enum class EMagnetFalloff : uint8
{
	None UMETA(DisplayName = "None"),
	Linear UMETA(DisplayName = "Linear"),
	InverseSquare UMETA(DisplayName = "Inverse Square")
};

UENUM(BlueprintType)
enum class EMagnetPolarity : uint8
{
	North UMETA(DisplayName = "North"),
	South UMETA(DisplayName = "South")
};

/**
 * プレイヤーや物理オブジェクトに磁力を与えるコンポーネント。
 *
 * N極 / S極を持つ
 * 同極なら反発
 * 異極なら吸着
 * 磁力範囲に入った瞬間に強いSnap
 * 範囲内では弱い継続磁力
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

	virtual void TickComponent(
		float DeltaTime,
		enum ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;


	//==================================================
	// 基本設定
	//==================================================

	/** 磁力の有効 / 無効 */
	UFUNCTION(BlueprintCallable, Category = "Magnet")
	void SetMagnetActive(bool bOn);

	/** N極 / S極を設定 */
	UFUNCTION(BlueprintCallable, Category = "Magnet")
	void SetPolarity(EMagnetPolarity InPolarity)
	{
		Polarity = InPolarity;
	}

	/** 磁力の基本強度を設定 */
	UFUNCTION(BlueprintCallable, Category = "Magnet")
	void SetStrength(float InStrength)
	{
		MagnetStrength = InStrength;
	}


	//==================================================
	// 磁力パラメータ
	//==================================================

	/** 磁力が届く半径 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet")
	float Radius = 600.f;

	/** 磁力の基本強度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet")
	float MagnetStrength = 2000.f;

	/** 磁力の距離減衰 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet")
	EMagnetFalloff Falloff = EMagnetFalloff::Linear;

	/** この磁石の極 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet")
	EMagnetPolarity Polarity = EMagnetPolarity::North;


	//==================================================
	// スナップ / 継続磁力
	//==================================================

	/**
	 * 磁力範囲に入った瞬間に与える強さ。
	 * 大きいほど「バンッ」と強く動く。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet|Snap")
	float SnapPower = 900.f;

	/**
	 * 磁力範囲内にいる間に与える弱い継続力。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet|Snap")
	float ContinuousPower = 250.f;


	//==================================================
	// 対象
	//==================================================

	/** 物理オブジェクトにも磁力を与える */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet")
	bool bAffectPhysics = true;

	/** Characterにも磁力を与える */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet")
	bool bAffectCharacters = true;

	/** 磁力範囲を検索するCollision Channel */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet")
	TEnumAsByte<ECollisionChannel> QueryChannel = ECC_PhysicsBody;


	//==================================================
	// デバッグ
	//==================================================

	/** 磁力範囲をデバッグ表示する */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet|Debug")
	bool bDrawDebug = false;

	/** デバッグ表示時間 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magnet|Debug")
	float DebugDrawTime = 0.1f;


protected:

	//==================================================
	// 内部状態
	//==================================================

	/** 磁力が現在有効か */
	bool bIsActive = true;

	/**
	 * 前フレームまで磁力範囲内にいたActor。
	 *
	 * 「初めて範囲に入った瞬間」を
	 * 検出するために使用する。
	 */
	TSet<TWeakObjectPtr<AActor>> InsideActors;


	//==================================================
	// 内部処理
	//==================================================

	/** 距離による磁力倍率 */
	float GetFalloffMultiplier(float Distance) const;

	/** N/Sの組み合わせから吸着か反発かを判定 */
	bool ShouldRepel(AActor* OtherActor) const;

	/** 磁力範囲に入った瞬間の強いSnap */
	void ApplySnap(ACharacter* Character, bool bRepel);

	/** 範囲内にいる間の弱い継続磁力 */
	void ApplyContinuous(
		ACharacter* Character,
		const FVector& Direction,
		bool bRepel,
		float DeltaTime
	);

	/** 磁力範囲内の対象を検索して処理 */
	void ApplyMagnetForces(float DeltaTime);
};