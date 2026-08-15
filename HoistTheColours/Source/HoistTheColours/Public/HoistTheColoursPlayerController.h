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

public:
	AHoistTheColoursPlayerController();
};