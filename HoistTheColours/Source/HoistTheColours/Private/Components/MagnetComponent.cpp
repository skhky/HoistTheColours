#include "Components/MagnetComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "CollisionQueryParams.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"

UMagnetComponent::UMagnetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetComponentTickEnabled(true);

	// シーンコンポーネントは移動可能にしておく（必要に応じて変更）
	SetMobility(EComponentMobility::Movable);

	bIsActive = true;
}

void UMagnetComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMagnetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UMagnetComponent::SetMagnetActive(bool bOn)
{
	// 親の SetActive を呼び、Tick の有効/無効も更新
	Super::SetActive(bOn);
	bIsActive = bOn;
	SetComponentTickEnabled(bOn);
}

void UMagnetComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsActive || Radius <= 0.f)
	{
		return;
	}

	ApplyMagnetForces(DeltaTime);
}

float UMagnetComponent::GetFalloffMultiplier(float Distance) const
{
	if (Distance <= KINDA_SMALL_NUMBER)
	{
		return 1.0f;
	}

	switch (Falloff)
	{
	case EMagnetFalloff::None:
		return 1.0f;
	case EMagnetFalloff::Linear:
		return FMath::Clamp(1.0f - (Distance / FMath::Max(Radius, KINDA_SMALL_NUMBER)), 0.0f, 1.0f);
	case EMagnetFalloff::InverseSquare:
	default:
	{
		const float Norm = Distance / FMath::Max(Radius, KINDA_SMALL_NUMBER);
		return 1.0f / FMath::Max(1.0f, Norm * Norm * 4.0f);
	}
	}
}

void UMagnetComponent::ApplyMagnetForces(float DeltaTime)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// コンポーネントのワールド位置（USceneComponent なのでコンポーネント固有の位置が使われる）
	const FVector Origin = GetComponentLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(MagnetOverlap), false, GetOwner());

	World->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, QueryChannel, Sphere, Params);

	if (bDrawDebug)
	{
		DrawDebugSphere(World, Origin, Radius, 16, FColor::Purple, false, DebugDrawTime, 0, 2.0f);
	}

	for (const FOverlapResult& Result : Overlaps)
	{
		UPrimitiveComponent* Prim = Result.GetComponent();
		AActor* OtherActor = Result.GetActor();
		if (!OtherActor || !Prim)
		{
			continue;
		}

		// 自身は無視
		if (OtherActor == GetOwner())
		{
			continue;
		}

		// 実際のコンポーネント位置を使う
		const FVector TargetLocation = Prim->GetComponentLocation();
		FVector ToOther = TargetLocation - Origin;
		const float Dist = ToOther.Size();
		if (Dist <= KINDA_SMALL_NUMBER)
		{
			continue;
		}

		FVector Dir = ToOther / Dist;
		const float Polarity = bRepel ? 1.0f : -1.0f;
		const float FalloffMul = GetFalloffMultiplier(Dist);
		const float ForceMag = MagnetStrength * FalloffMul;

		// 最終的な力ベクトル
		FVector Force = Dir * ForceMag * Polarity;

		// 物理シミュレーションされているコンポーネントには AddForce
		if (bAffectPhysics && Prim->IsSimulatingPhysics())
		{
			Prim->AddForce(Force, NAME_None, true);
			if (bDrawDebug)
			{
				DrawDebugDirectionalArrow(World, Prim->GetComponentLocation(), Prim->GetComponentLocation() + Force.GetSafeNormal() * FMath::Min(200.f, Force.Size() * 0.01f), 20, FColor::Blue, false, DebugDrawTime, 0, 2.0f);
			}
			continue;
		}

		// キャラクターには簡易 Launch を適用
		if (bAffectCharacters)
		{
			if (ACharacter* Ch = Cast<ACharacter>(OtherActor))
			{
				// 小さめのスケールでフレーム依存性を補正
				const FVector LaunchVel = Force * DeltaTime * 0.01f;
				Ch->LaunchCharacter(LaunchVel, true, true);
				if (bDrawDebug)
				{
					DrawDebugDirectionalArrow(World, Ch->GetActorLocation(), Ch->GetActorLocation() + LaunchVel * 0.1f, 20, FColor::Green, false, DebugDrawTime, 0, 2.0f);
				}
				continue;
			}
		}

		// フォールバックで AddForce を試す
		if (bAffectPhysics && Prim)
		{
			Prim->AddForce(Force, NAME_None, true);
		}
	}
}