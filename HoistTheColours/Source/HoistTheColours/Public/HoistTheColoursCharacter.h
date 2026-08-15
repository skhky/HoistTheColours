// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HoistTheColoursCharacter.generated.h"

/**
 *  A controllable top-down perspective character
 */
UCLASS(abstract)
class AHoistTheColoursCharacter : public ACharacter
{
	GENERATED_BODY()

private:

public:

	/** Constructor */
	AHoistTheColoursCharacter();

	/** Initialization */
	virtual void BeginPlay() override;

	/** Update */
	virtual void Tick(float DeltaSeconds) override;

};

