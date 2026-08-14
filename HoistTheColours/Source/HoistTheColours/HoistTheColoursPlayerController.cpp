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
#include "Components/TextBlock.h"

AHoistTheColoursPlayerController::AHoistTheColoursPlayerController()
{
	isPolarityPressed = false;
}

void AHoistTheColoursPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalPlayerController())
	{
		if (InputComponent)
		{
			InputComponent->BindAxis(TEXT("MoveForward"), this, &AHoistTheColoursPlayerController::MoveForward);
			InputComponent->BindAxis(TEXT("MoveRight"), this, &AHoistTheColoursPlayerController::MoveRight);
			InputComponent->BindAxis(TEXT("ActionPolarity"), this, &AHoistTheColoursPlayerController::ActionPolarity);
		}
	}
}


//------------------------------------------------------------------------------
// Action
//------------------------------------------------------------------------------
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

	const bool bIsPressedNow = FMath::Abs(value) >= DeadZone;

	if (bIsPressedNow)
	{
		if (isPolarityPressed)
		{
			return;
		}

		isPolarityPressed = true;

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

		// HUDXV
		if (AHoistTheColoursCharacter* HTC = Cast<AHoistTheColoursCharacter>(ControlledPawn))
		{
			if (UTextBlock* TB = HTC->GetPlayerHUDTextBlock(TEXT("UpperMagnetText")))
			{
				const FText NewText = (changePolarity == EMagnetPolarity::South)
					? FText::FromString(TEXT("S"))
					: FText::FromString(TEXT("N"));

				TB->SetText(NewText);
			}
		}

		return;
	}

	isPolarityPressed = false;
}
