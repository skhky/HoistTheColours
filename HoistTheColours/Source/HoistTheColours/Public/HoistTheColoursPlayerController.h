#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HoistTheColoursPlayerController.generated.h"

class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class UPathFollowingComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(abstract)
class AHoistTheColoursPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	bool isPolarityPressed;

public:
	AHoistTheColoursPlayerController();

protected:
	virtual void SetupInputComponent() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void ActionPolarity(float Value);
};