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

	SetMobility(EComponentMobility::Movable);

	bIsActive = true;
}


void UMagnetComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UMagnetComponent::EndPlay(
	const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


void UMagnetComponent::SetMagnetActive(bool bOn)
{
	// ActorComponentとしてのActive状態
	Super::SetActive(bOn);

	bIsActive = bOn;

	// Tickも同時にON/OFF
	SetComponentTickEnabled(bOn);
}


void UMagnetComponent::TickComponent(
	float DeltaTime,
	enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(
		DeltaTime,
		TickType,
		ThisTickFunction);

	// 磁力が無効、または半径が0なら何もしない
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
		return FMath::Clamp(
			1.0f - (Distance / FMath::Max(Radius, KINDA_SMALL_NUMBER)),
			0.0f,
			1.0f
		);

	case EMagnetFalloff::InverseSquare:
	default:
	{
		const float Norm =
			Distance / FMath::Max(Radius, KINDA_SMALL_NUMBER);

		return 1.0f /
			FMath::Max(
				1.0f,
				Norm * Norm * 4.0f
			);
	}
	}
}


bool UMagnetComponent::ShouldRepel(AActor* OtherActor) const
{
	if (!OtherActor)
	{
		return false;
	}

	// 相手が磁石コンポーネントを持っているか確認
	const UMagnetComponent* OtherMagnet =
		OtherActor->FindComponentByClass<UMagnetComponent>();

	if (!OtherMagnet)
	{
		return false;
	}

	// 同じ極なら反発
	// N-N → 反発
	// S-S → 反発
	//
	// 違う極なら吸着
	// N-S → 吸着
	// S-N → 吸着
	return Polarity == OtherMagnet->Polarity;
}


void UMagnetComponent::ApplySnap(
	ACharacter* Character,
	bool bRepel)
{
	if (!Character)
	{
		return;
	}

	// 自分 → 相手 の方向
	FVector Direction =
		(Character->GetActorLocation() - GetComponentLocation())
		.GetSafeNormal();

	if (Direction.IsNearlyZero())
	{
		return;
	}

	// 同極なら反発なので方向を逆にする
	if (bRepel)
	{
		Direction *= -1.0f;
	}

	// 範囲に入った瞬間だけ強く飛ばす
	const FVector LaunchVelocity =
		Direction * SnapPower;

	Character->LaunchCharacter(
		LaunchVelocity,
		true,
		true);

	// デバッグ表示
	if (bDrawDebug)
	{
		UWorld* World = GetWorld();

		if (World)
		{
			DrawDebugDirectionalArrow(
				World,
				Character->GetActorLocation(),
				Character->GetActorLocation() +
				LaunchVelocity * 0.2f,
				30.0f,
				FColor::Red,
				false,
				DebugDrawTime,
				0,
				3.0f
			);
		}
	}
}


void UMagnetComponent::ApplyContinuous(
	ACharacter* Character,
	const FVector& Direction,
	bool bRepel,
	float DeltaTime)
{
	if (!Character)
	{
		return;
	}

	FVector ForceDirection = Direction;

	if (ForceDirection.IsNearlyZero())
	{
		return;
	}

	// 同極なら反発
	if (bRepel)
	{
		ForceDirection *= -1.0f;
	}

	UCharacterMovementComponent* Movement =
		Character->GetCharacterMovement();

	if (!Movement)
	{
		return;
	}

	// 弱い継続磁力
	Movement->Velocity +=
		ForceDirection *
		ContinuousPower *
		DeltaTime;
}


void UMagnetComponent::ApplyMagnetForces(float DeltaTime)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	//==================================================
	// 磁力範囲
	//==================================================

	const FVector Origin =
		GetComponentLocation();

	FCollisionShape Sphere =
		FCollisionShape::MakeSphere(Radius);

	TArray<FOverlapResult> Overlaps;

	FCollisionQueryParams Params(
		SCENE_QUERY_STAT(MagnetOverlap),
		false,
		GetOwner()
	);


	//==================================================
	// 範囲内のオブジェクトを取得
	//==================================================

	World->OverlapMultiByChannel(
		Overlaps,
		Origin,
		FQuat::Identity,
		QueryChannel,
		Sphere,
		Params
	);


	//==================================================
	// デバッグ：磁力範囲
	//==================================================

	if (bDrawDebug)
	{
		DrawDebugSphere(
			World,
			Origin,
			Radius,
			16,
			FColor::Purple,
			false,
			DebugDrawTime,
			0,
			2.0f
		);
	}


	//==================================================
	// 今フレーム範囲内にいるActor
	//==================================================

	TSet<TWeakObjectPtr<AActor>> CurrentInsideActors;


	//==================================================
	// 範囲内のActorを処理
	//==================================================

	for (const FOverlapResult& Result : Overlaps)
	{
		UPrimitiveComponent* Prim =
			Result.GetComponent();

		AActor* OtherActor =
			Result.GetActor();

		if (!OtherActor || !Prim)
		{
			continue;
		}


		// 自分自身は無視
		if (OtherActor == GetOwner())
		{
			continue;
		}


		//==================================================
		// 相手が磁石か確認
		//==================================================

		UMagnetComponent* OtherMagnet =
			OtherActor->FindComponentByClass<UMagnetComponent>();

		if (!OtherMagnet)
		{
			// 磁石を持っていないオブジェクトは
			// Characterとして処理することも可能だが、
			// 今回はスキップ
			continue;
		}


		//==================================================
		// 距離・方向
		//==================================================

		const FVector TargetLocation =
			OtherActor->GetActorLocation();

		FVector ToOther =
			TargetLocation - Origin;

		const float Distance =
			ToOther.Size();

		if (Distance <= KINDA_SMALL_NUMBER)
		{
			continue;
		}

		const FVector Direction =
			ToOther / Distance;


		//==================================================
		// N/Sによる吸着・反発判定
		//==================================================

		const bool bRepel =
			ShouldRepel(OtherActor);


		//==================================================
		// 現在の磁力
		//==================================================

		const float FalloffMultiplier =
			GetFalloffMultiplier(Distance);

		const float ForceMagnitude =
			MagnetStrength *
			FalloffMultiplier;


		//==================================================
		// 現在フレームの力の方向
		//==================================================

		FVector ForceDirection =
			Direction;

		if (bRepel)
		{
			ForceDirection *= -1.0f;
		}


		const FVector Force =
			ForceDirection *
			ForceMagnitude;


		//==================================================
		// 今フレーム範囲内として記録
		//==================================================

		CurrentInsideActors.Add(OtherActor);


		//==================================================
		// Character処理
		//==================================================

		if (bAffectCharacters)
		{
			ACharacter* Character =
				Cast<ACharacter>(OtherActor);

			if (Character)
			{
				// 前フレームから範囲内にいたか？
				const bool bWasInside =
					InsideActors.Contains(OtherActor);


				//==========================================
				// 初めて範囲に入った
				//==========================================

				if (!bWasInside)
				{
					ApplySnap(
						Character,
						bRepel
					);
				}


				//==========================================
				// 範囲内にいる間
				//==========================================

				ApplyContinuous(
					Character,
					Direction,
					bRepel,
					DeltaTime
				);


				// デバッグ
				if (bDrawDebug)
				{
					DrawDebugDirectionalArrow(
						World,
						Character->GetActorLocation(),
						Character->GetActorLocation() +
						ForceDirection *
						FMath::Min(
							200.0f,
							ForceMagnitude * 0.01f
						),
						20.0f,
						FColor::Green,
						false,
						DebugDrawTime,
						0,
						2.0f
					);
				}

				continue;
			}
		}


		//==================================================
		// Physicsオブジェクト
		//==================================================

		if (bAffectPhysics &&
			Prim->IsSimulatingPhysics())
		{
			Prim->AddForce(
				Force,
				NAME_None,
				true
			);

			if (bDrawDebug)
			{
				DrawDebugDirectionalArrow(
					World,
					Prim->GetComponentLocation(),
					Prim->GetComponentLocation() +
					Force.GetSafeNormal() *
					FMath::Min(
						200.0f,
						Force.Size() * 0.01f
					),
					20.0f,
					FColor::Blue,
					false,
					DebugDrawTime,
					0,
					2.0f
				);
			}

			continue;
		}
	}


	//==================================================
	// 今フレームの範囲内情報を保存
	//==================================================

	InsideActors =
		MoveTemp(CurrentInsideActors);
}
