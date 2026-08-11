// Copyright Epic Games, Inc. All Rights Reserved.

#include "HoistTheColoursPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "HoistTheColoursCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "HoistTheColours.h"
#include "Components/InputComponent.h"
#include "Components/MagnetComponent.h"

AHoistTheColoursPlayerController::AHoistTheColoursPlayerController()
{
	bIsTouch = false;
	bMoveToMouseCursor = false;

	PathFollowingComponent = CreateDefaultSubobject<UPathFollowingComponent>(TEXT("Path Following Component"));

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AHoistTheColoursPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{

			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AHoistTheColoursPlayerController::OnInputStarted);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AHoistTheColoursPlayerController::OnSetDestinationTriggered);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AHoistTheColoursPlayerController::OnSetDestinationReleased);
			EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AHoistTheColoursPlayerController::OnSetDestinationReleased);

			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AHoistTheColoursPlayerController::OnInputStarted);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AHoistTheColoursPlayerController::OnTouchTriggered);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AHoistTheColoursPlayerController::OnTouchReleased);
			EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AHoistTheColoursPlayerController::OnTouchReleased);
		}
		else
		{
			UE_LOG(LogHoistTheColours, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
		}

		if (InputComponent)
		{
			InputComponent->BindAxis(TEXT("MoveForward"), this, &AHoistTheColoursPlayerController::MoveForward);
			InputComponent->BindAxis(TEXT("MoveRight"), this, &AHoistTheColoursPlayerController::MoveRight);
			InputComponent->BindAxis(TEXT("ActionPolarity"), this, &AHoistTheColoursPlayerController::ActionPolarity);
		}
	}
}

void AHoistTheColoursPlayerController::OnInputStarted()
{
	StopMovement();
	UpdateCachedDestination();
}

void AHoistTheColoursPlayerController::OnSetDestinationTriggered()
{
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	UpdateCachedDestination();
	
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AHoistTheColoursPlayerController::OnSetDestinationReleased()
{
	if (FollowTime <= ShortPressThreshold)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

void AHoistTheColoursPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AHoistTheColoursPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AHoistTheColoursPlayerController::UpdateCachedDestination()
{
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
}

void AHoistTheColoursPlayerController::MoveForward(float Value)
{
	if (FMath::IsNearlyZero(Value))
	{
		return;
	}

	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn == nullptr)
	{
		return;
	}

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	ControlledPawn->AddMovementInput(Direction, Value);
}

void AHoistTheColoursPlayerController::MoveRight(float Value)
{
	if (FMath::IsNearlyZero(Value))
	{
		return;
	}

	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn == nullptr)
	{
		return;
	}

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	ControlledPawn->AddMovementInput(Direction, Value);
}

void AHoistTheColoursPlayerController::ActionPolarity(float value)
{
	const float DeadZone = 0.1f;
	if (FMath::Abs(value) < DeadZone)
	{
		return;
	}

	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn == nullptr)
	{
		return;
	}

	UMagnetComponent* Magnet = ControlledPawn->FindComponentByClass<UMagnetComponent>();
	if (Magnet == nullptr)
	{
		return;
	}

	auto changePolarity = Magnet->Polarity == EMagnetPolarity::North ? EMagnetPolarity::South : EMagnetPolarity::North;
	Magnet->SetPolarity(changePolarity);
}
